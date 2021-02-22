#ifndef __SIM_H__
#define __SIM_H__

#include "platform.h"
#include "calc.h"
#include "object.h"

#define N_Building 10

#define NL_Building 20
#define NR_Building 20

#define N_particle 10

// sim.cpp
void InitScene( void );
void UpdateScene( void );
void TermScene( void );

// draw.cpp
void PreDraw( void );
void PostDraw( void );
void DrawScene( void );
//void Lighting( void );
//void HeadLight( void );
/*
��ObjDataT�̒�`�́Aobject.h�Ɉڍs���܂���
*/
//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ���[�U��`�̃V�[���ɂ��Ẵf�[�^�������ɂ܂Ƃ߂�

	ObjDataT player; /////���v���C���i�̑������j������킷�I�u�W�F�N�g

	ObjDataT head;//��
	ObjDataT body;
	ObjDataT handL;//��
	ObjDataT handR;//��
	ObjDataT footL;//������������Ⴂ������Ȃ�����
	ObjDataT footR;

	ObjDataT *active_camera; //�J�����̃|�C���^�ϐ�

	float viewing = 20;

	////////
} SimDataT;

#endif //__SIM_H__
