#ifndef __CALC_H__
#define __CALC_H__

#include <cmath>
#include "vector.h"
#include "euler.h"
#include "color.h"
#include "matrix.h"
#include "object.h"

///////////////////////////////
//▼オリエンテーション構造体
typedef struct {
	float x, y, z, angle;
} OrientationT;
//▼MREALターゲット構造体
typedef struct {
	vector_t pos;
	OrientationT ori;
	int state;
	bool detected;
}TargetT;

//▼MREALマーカ構造体
typedef struct {
	vector_t pos;
	OrientationT ori;
	int state; //前フレームからのdetected状態の変化
	bool detected;
	int markerID;
	int targetID;
} MarkerT;

void vector_fromAngle2d(float angle, vector_t *v);

void setObjPos(ObjDataT *obj, float *pos);
void setObjRot(ObjDataT *obj, float *rot);
void getObjPos(ObjDataT *obj, float *pos);
void getObjRot(ObjDataT *obj, float *rot);
void copyObj(ObjDataT *src, ObjDataT *dst);

void setObjColor(ObjDataT *obj, float red, float green, float blue);
void setObjPos(ObjDataT *obj, float x, float y, float z);
void setObjRot(ObjDataT *obj, float roll, float pitch, float yaw);

void DirectionAtoB(ObjDataT *a, ObjDataT *b, vector_t *dir);
float DistanceAtoB(ObjDataT *a, ObjDataT *b);
float VectorToPolar(vector_t *v, euler_t *angle);
void PolarToVector(euler_t *angle, vector_t *v);
float EulerAtoB(ObjDataT *a, ObjDataT *b, euler_t *angle);

void MoveObject(ObjDataT *obj);

float uniformRandom(float min, float max);
float gaussianRandom(float rmin, float rmax);

void setObjLocal(ObjDataT *target, ObjDataT *base);
void setObjWorld(ObjDataT *target);
void moveLocalToWorld(ObjDataT *target);
void moveWorldToLocal(ObjDataT *target, ObjDataT *base);

bool HitTest(ObjDataT *a, ObjDataT *b);
//a： 衝突判定の相手
//b： 衝突判定の主体
//戻り値： 衝突した場合にtrueが返る
bool HitTestBox(ObjDataT *box, ObjDataT *ball);
bool isHit(ObjDataT *a, ObjDataT *b);
bool isHitBox(ObjDataT *box, ObjDataT *ball);

//ローカル座標系からワールド座標系への変換
void TransformLocalToWorld(ObjDataT *base, ObjDataT *local, ObjDataT *world);
//ワールド座標系からローカル座標系への変換
void TransformWorldToLocal(ObjDataT *base, ObjDataT *world, ObjDataT *local);

//---- ターゲット処理用関数
void TargetToObjData(TargetT *src, ObjDataT *obj);
void TransformLocalToWorldX(TargetT *base, ObjDataT *local, ObjDataT *world);

//---- ベクトル処理

void DirectionLocalToWorld(ObjDataT * target, vector_t * localDir, vector_t * worldDir);

void LocalForward(ObjDataT *target, vector_t *forward);

void LocalUP(ObjDataT * target, vector_t *up);


//---- apply~~ 
void applyMaterialColor(float r, float g, float b);
//▼オブジェクトの位置と姿勢の幾何変換を行う関数
void applyObjTransform(ObjDataT *obj);
//▼オブジェクトのカラーを指定する関数
void applyObjColor(ObjDataT *obj);

#endif
