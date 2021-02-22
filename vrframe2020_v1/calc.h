#ifndef __CALC_H__
#define __CALC_H__

#include <cmath>
#include "vector.h"
#include "euler.h"
#include "color.h"
#include "matrix.h"
#include "object.h"

///////////////////////////////
//���I���G���e�[�V�����\����
typedef struct {
	float x, y, z, angle;
} OrientationT;
//��MREAL�^�[�Q�b�g�\����
typedef struct {
	vector_t pos;
	OrientationT ori;
	int state;
	bool detected;
}TargetT;

//��MREAL�}�[�J�\����
typedef struct {
	vector_t pos;
	OrientationT ori;
	int state; //�O�t���[�������detected��Ԃ̕ω�
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
//a�F �Փ˔���̑���
//b�F �Փ˔���̎��
//�߂�l�F �Փ˂����ꍇ��true���Ԃ�
bool HitTestBox(ObjDataT *box, ObjDataT *ball);
bool isHit(ObjDataT *a, ObjDataT *b);
bool isHitBox(ObjDataT *box, ObjDataT *ball);

//���[�J�����W�n���烏�[���h���W�n�ւ̕ϊ�
void TransformLocalToWorld(ObjDataT *base, ObjDataT *local, ObjDataT *world);
//���[���h���W�n���烍�[�J�����W�n�ւ̕ϊ�
void TransformWorldToLocal(ObjDataT *base, ObjDataT *world, ObjDataT *local);

//---- �^�[�Q�b�g�����p�֐�
void TargetToObjData(TargetT *src, ObjDataT *obj);
void TransformLocalToWorldX(TargetT *base, ObjDataT *local, ObjDataT *world);

//---- �x�N�g������

void DirectionLocalToWorld(ObjDataT * target, vector_t * localDir, vector_t * worldDir);

void LocalForward(ObjDataT *target, vector_t *forward);

void LocalUP(ObjDataT * target, vector_t *up);


//---- apply~~ 
void applyMaterialColor(float r, float g, float b);
//���I�u�W�F�N�g�̈ʒu�Ǝp���̊􉽕ϊ����s���֐�
void applyObjTransform(ObjDataT *obj);
//���I�u�W�F�N�g�̃J���[���w�肷��֐�
void applyObjColor(ObjDataT *obj);

#endif
