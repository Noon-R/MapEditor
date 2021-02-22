#include "platform.h"

#include <math.h>
#include <stdlib.h> //rand

#include "sim.h"
#include "calc.h"
#include "ezUtil.h"

//float PI = 3.14159;
const float RADIAN = M_PI / 180.0;
const float DEGREE = 180.0 / M_PI;

void vector_fromAngle2d(float angle, vector_t *v)
{
	v->x = cosf(RADIAN * angle);
	v->y = sinf(RADIAN * angle);
	v->z = 0.0;
}

void setObjPos(ObjDataT *obj, float *pos)
{
	obj->pos.x = pos[0];
	obj->pos.y = pos[1];
	obj->pos.z = pos[2];
}
void setObjRot(ObjDataT *obj, float *rot)
{
	obj->rot.roll = rot[0];
	obj->rot.pitch = rot[1];
	obj->rot.yaw = rot[2];
}
void getObjPos(ObjDataT *obj, float *pos)
{
	pos[0] = obj->pos.x;
	pos[1] = obj->pos.y;
	pos[2] = obj->pos.z;
}
void getObjRot(ObjDataT *obj, float *rot)
{
	rot[0] = obj->rot.roll;
	rot[1] = obj->rot.pitch;
	rot[2] = obj->rot.yaw;
}

void setObjColor(ObjDataT *obj, float red, float green, float blue)
{
	obj->color.red = red;
	obj->color.green = green;
	obj->color.blue = blue;
}
void setObjPos(ObjDataT *obj, float x, float y, float z)
{
	obj->pos.x = x;
	obj->pos.y = y;
	obj->pos.z = z;
}
void setObjRot(ObjDataT *obj, float roll, float pitch, float yaw)
{
	obj->rot.roll = roll;
	obj->rot.pitch = pitch;
	obj->rot.yaw = yaw;
}

void copyObj(ObjDataT *src, ObjDataT *dst)
{
	setObjPos(dst, src->pos.x, src->pos.y, src->pos.z);
	setObjRot(dst, src->rot.roll, src->rot.pitch, src->rot.yaw);
	setObjColor(dst, src->color.red, src->color.green, src->color.blue);
	dst->base = src->base;
	dst->radius = src->radius;
	dst->state = src->state;
	dst->visible = src->visible;
	dst->move = src->move;
	dst->turn = src->turn;
	dst->xsize = src->xsize;
	dst->ysize = src->ysize;
	dst->zsize = src->zsize;
	return;
}

//▼オブジェクトaからオブジェクトbへの方向ベクトルを求める関数
//オブジェクト間の方向ベクトル
void DirectionAtoB(ObjDataT *a, ObjDataT *b, vector_t *dir)
{
	dir->x = b->pos.x - a->pos.x;
	dir->y = b->pos.y - a->pos.y;
	dir->z = b->pos.z - a->pos.z;
}
//▼距離を求める関数（「衝突判定」で作った関数の別バージョン）
//オブジェクト間距離
float DistanceAtoB(ObjDataT *a, ObjDataT *b)
{
	vector_t v;
	DirectionAtoB(a, b, &v);
	return VectorMagnitude(v);
}
//▼方向ベクトルから方位角と仰角を求める関数
float VectorToPolar(vector_t *v, euler_t *angle)
{
	float d = sqrtf(v->x * v->x + v->z * v->z);
	angle->pitch = DEGREE * atan2f(v->y, d);
	angle->yaw = DEGREE * atan2f(-v->x, -v->z);
	return VectorMagnitude(*v);
}
//▼方位角と仰角から単位方向ベクトルを求める関数
void PolarToVector(euler_t *angle, vector_t *v)
{
	float rad, l;

	rad = angle->pitch * RADIAN;
	v->y = sinf(rad);
	l = cosf(rad);

	rad = angle->yaw * RADIAN;
	v->x = -l * sinf(rad);
	v->z = -l * cosf(rad);

	return;
}
//▼オブジェクト間の角度（aからbを見込む方位角と仰角）
float EulerAtoB(ObjDataT *a, ObjDataT *b, euler_t *angle)
{
	vector_t dir;
	DirectionAtoB(a, b, &dir);
	VectorToPolar(&dir, angle);
	return VectorMagnitude(dir);
}
//▼備考：構造体へのポインタを引数としている
void MoveObject(ObjDataT *obj)
{
	obj->rot.yaw += obj->turn;
	obj->pos.x -= obj->move * sinf(obj->rot.yaw * RADIAN);
	obj->pos.z -= obj->move * cosf(obj->rot.yaw * RADIAN);
	obj->pos.y += obj->move * sinf(obj->rot.pitch * RADIAN); ///// y方向の移動も考慮する場合
}
//▼物体同士の距離に基づいて衝突したかをチェックする関数
bool HitTest(ObjDataT *a, ObjDataT *b)
{
	if (DistanceAtoB(a, b) < a->radius + b->radius) return true;
	else return false;
}
//▼箱型オブジェクト（領域）と球状オブジェクト（領域）の衝突判定
bool HitTestBox(ObjDataT *box, ObjDataT *ball)
{

	//箱の座標範囲
	float xmin = box->pos.x - box->xsize / 2.0;
	float xmax = box->pos.x + box->xsize / 2.0;
	float ymin = box->pos.y - box->ysize / 2.0;
	float ymax = box->pos.y + box->ysize / 2.0;
	float zmin = box->pos.z - box->zsize / 2.0;
	float zmax = box->pos.z + box->zsize / 2.0;
	//X座標の衝突判定
	if (xmax < ball->pos.x - ball->radius || xmin > ball->pos.x + ball->radius) {
		return false;
	}
	//Y座標の衝突判定
	if (ymax < ball->pos.y - ball->radius || ymin > ball->pos.y + ball->radius) {
		return false;
	}
	//Z座標の衝突判定
	if (zmax < ball->pos.z - ball->radius || zmin > ball->pos.z + ball->radius) {
		return false;
	}
	return true;
}

bool isHit(ObjDataT *a, ObjDataT *b)
{
	ObjDataT a_world, b_world;

	copyObj(a, &a_world);
	copyObj(b, &b_world);
	moveLocalToWorld(&a_world);
	moveLocalToWorld(&b_world);

	return HitTest(&a_world, &b_world);
}

bool isHitBox(ObjDataT *box, ObjDataT *ball)
{
	ObjDataT ball_world;
	ObjDataT box_world;

	copyObj(box, &box_world);
	copyObj(ball, &ball_world);
	moveLocalToWorld(&box_world);
	moveLocalToWorld(&ball_world);
	moveWorldToLocal(&ball_world, &box_world);

	return HitTestBox(&box_world, &ball_world);
}

//====================================================================================================
//▼乱数生成関数
//最小値minと最大値maxの間の値をランダムに返す
float uniformRandom(float min, float max) {
	double r;
	r = (double)rand() / (double)RAND_MAX;
	r = r * (max - min) + min;
	return r;
}
//▼簡易ガウスノイズ関数
float gaussianRandom(float rmin, float rmax) {
	int i;
	float r = 0.0;
	const int n = 12;
	for (i = 0; i < n; i++) {
		r += uniformRandom(rmin, rmax);
	}
	return r / n;
}
//-------------座標系階層構造操作関数
// setObjLocal
void setObjLocal(ObjDataT *target, ObjDataT *base)
{
	target->base = base;
	return;
}
// setObjWorld
void setObjWorld(ObjDataT *target)
{
	target->base = NULL;
	return;
}
//--------------- 
//ローカルからワールドへ
void moveLocalToWorld(ObjDataT *target)
{
	ObjDataT *base;
	base = target->base;
	while (base != NULL) {
		TransformLocalToWorld(base, target, target);
		base = base->base;
		setObjLocal(target, base);
	}
	setObjWorld(target);
	return;
}
//ワールドからローカルへ
void moveWorldToLocal(ObjDataT *target, ObjDataT *base)
{
	ObjDataT *b, world;

	b = base->base;
	if (b != NULL) {
		world.pos.x = base->pos.x;
		world.pos.y = base->pos.y;
		world.pos.z = base->pos.z;
		world.rot.roll = base->rot.roll;
		world.rot.pitch = base->rot.pitch;
		world.rot.yaw = base->rot.yaw;
		world.base = base->base;
		moveLocalToWorld(&world);
	}
	TransformWorldToLocal(&world, target, target);
	setObjLocal(target, base);
	return;
}
//----------------------------------- LocalToWorld
void TransformLocalToWorld(ObjDataT *base, ObjDataT *local, ObjDataT *world)
{
	//ObjDataT hand, target;
	float base_pos[3], base_rot[3], local_pos[3], local_rot[3];
	float world_pos[3], world_rot[3];

	base_pos[0] = base->pos.x;
	base_pos[1] = base->pos.y;
	base_pos[2] = base->pos.z;
	base_rot[0] = base->rot.roll;
	base_rot[1] = base->rot.pitch;
	base_rot[2] = base->rot.yaw;

	local_pos[0] = local->pos.x;
	local_pos[1] = local->pos.y;
	local_pos[2] = local->pos.z;
	local_rot[0] = local->rot.roll;
	local_rot[1] = local->rot.pitch;
	local_rot[2] = local->rot.yaw;

	ezUtil_Mult(base_pos, base_rot, local_pos, local_rot, world_pos, world_rot);

	world->pos.x = world_pos[0];
	world->pos.y = world_pos[1];
	world->pos.z = world_pos[2];
	world->rot.roll = world_rot[0];
	world->rot.pitch = world_rot[1];
	world->rot.yaw = world_rot[2];
}

void TransformWorldToLocal(ObjDataT *base, ObjDataT *target, ObjDataT *local)
{

	matrix_t matrix;

	glPushMatrix();
	glLoadIdentity();
	glRotatef(-base->rot.roll, 0.0, 0.0, 1.0);
	glRotatef(-base->rot.pitch, 1.0, 0.0, 0.0);
	glRotatef(-base->rot.yaw, 0.0, 1.0, 0.0);
	glTranslatef(-base->pos.x,
		-base->pos.y,
		-base->pos.z);

	glTranslatef(target->pos.x, target->pos.y, target->pos.z);
	glRotatef(target->rot.yaw, 0.0, 1.0, 0.0);
	glRotatef(target->rot.pitch, 1.0, 0.0, 0.0);
	glRotatef(target->rot.roll, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();

	float rot[3], pos[3];
	ezUtil_getRot(matrix, rot);
	ezUtil_getPos(matrix, pos);
	local->pos.x = pos[0];
	local->pos.y = pos[1];
	local->pos.z = pos[2];
	local->rot.roll = rot[0];
	local->rot.pitch = rot[1];
	local->rot.yaw = rot[2];

	return;
}
//-----------------------------------------------
void TargetToObjData(TargetT *src, ObjDataT *dst)
{
	if (src->state == 1) {
		dst->pos.x = 100;
		dst->pos.y = 100;
		dst->pos.z = 100;
		dst->rot.roll = 0;
		dst->rot.pitch = 0;
		dst->rot.yaw = 0;
	}

	matrix_t matrix;
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(src->pos.x,
		src->pos.y,
		src->pos.z);

	glRotatef(src->ori.angle,
		src->ori.x,
		src->ori.y,
		src->ori.z);

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();

	float rot[3], pos[3];
	ezUtil_getRot(matrix, rot);
	ezUtil_getPos(matrix, pos);
	dst->pos.x = pos[0];
	dst->pos.y = pos[1];
	dst->pos.z = pos[2];
	dst->rot.roll = rot[0];
	dst->rot.pitch = rot[1];
	dst->rot.yaw = rot[2];

	return;
}
//--------
void TransformLocalToWorldX(TargetT *base, ObjDataT *local, ObjDataT *world)
{
	matrix_t matrix;
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base->pos.x,
		base->pos.y,
		base->pos.z);

	glRotatef(base->ori.angle,
		base->ori.x,
		base->ori.y,
		base->ori.z);


	glTranslatef(local->pos.x, local->pos.y, local->pos.z);
	glRotatef(local->rot.yaw, 0.0, 1.0, 0.0);
	glRotatef(local->rot.pitch, 1.0, 0.0, 0.0);
	glRotatef(local->rot.roll, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();

	float rot[3], pos[3];
	ezUtil_getRot(matrix, rot);
	ezUtil_getPos(matrix, pos);
	world->pos.x = pos[0];
	world->pos.y = pos[1];
	world->pos.z = pos[2];
	world->rot.roll = rot[0];
	world->rot.pitch = rot[1];
	world->rot.yaw = rot[2];

	return;
}

void LocalForward(ObjDataT * target, vector_t * forward)
{
	vector_t forWardDir = { 0,0,1 };
	DirectionLocalToWorld(target, &forWardDir, forward);
}

void LocalUP(ObjDataT * target, vector_t * up)
{
	vector_t upDir = { 0,1,0 };
	DirectionLocalToWorld(target, &upDir, up);
}


void DirectionLocalToWorld(ObjDataT * target, vector_t * localDir, vector_t * worldDir) {
	
	//*
	float pos_origin[3];
	float pos_dir[3];

	matrix_t matrix[2];
	glPushMatrix(); {
		glLoadIdentity();

		applyObjTransform(target);
		glTranslatef(0,0,0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix[0]);

		glTranslatef(localDir->x, localDir->y, localDir->z);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix[1]);
	}glPopMatrix();
	ezUtil_getPos(matrix[0], pos_origin);
	ezUtil_getPos(matrix[1], pos_dir);


	worldDir->x = pos_origin[0] - pos_dir[0];
	worldDir->y = pos_origin[1] - pos_dir[1];
	worldDir->z = pos_origin[2] - pos_dir[2];
	/*/
	ObjDataT dir;
	moveWorldToLocal(&dir, target);
	setObjPos(&dir,
		localDir->x,
		localDir->y,
		localDir->z);
	moveLocalToWorld(&dir);

	worldDir->x = dir.pos.x;
	worldDir->y = dir.pos.y;
	worldDir->z = dir.pos.z;
	//*/
	*worldDir = VectorNormalized(*worldDir);
}

//プログラミング上のトピックス
//関数の引数として「構造体のポインタ」を与えるには？（アロー演算子）
//「構造体のポインタ」を使って構造体の中身「メンバー変数」を使うには？
void applyObjTransform(ObjDataT *obj)
{
	if (obj->base != NULL) {
		applyObjTransform(obj->base);
	}
	glTranslatef(obj->pos.x, obj->pos.y, obj->pos.z); //位置
	glRotatef(obj->rot.yaw, 0.0, 1.0, 0.0);//ヨー（方位角）ｙ軸
	glRotatef(obj->rot.pitch, 1.0, 0.0, 0.0);//ピッチ（仰角）ｘ軸
	glRotatef(obj->rot.roll, 0.0, 0.0, 1.0);//ロール（傾き）ｚ軸
	return;
}

void applyObjColor(ObjDataT *obj)
{
	applyMaterialColor(obj->color.red, obj->color.green, obj->color.blue);
}

/*--------------------------------------------------------- setMaterialColor
 * setMaterialColor:
 *--------*/
void applyMaterialColor(float r, float g, float b)
{
	float diffuse[4];
	float specular[] = { 0, 0, 0, 1.0 };

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = 1.0;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	return;
}
