#pragma once
#include <vector>

#include "object.h"

#define CELL_SIZE 1

typedef struct {

	int field_width, field_height; //���C�Y�t�B�[���h���̏c���A���ڂ̌�
	std::vector<int> cells;
	std::vector<ObjDataT> cellObjs;

} ezMapDataT;

//���C�Y�f�[�^�����[�h����
//keyboard�R�[���o�b�N�֐��ŁA�Ⴆ��[L]�L�[�Ńf�[�^�����[�h����
bool ezMap_load(char const *file);

//���C�Y�f�[�^���Z�[�u����
bool ezMap_save(char const *file);

//���[�h�������C�Y�f�[�^���v�����g�Ŋm�F����
void ezMap_print();

//���C�Y�f�[�^������������
//���[�h���ꂽ���C�Y�f�[�^�����ɂ��āA�`��p�̃f�[�^�𐶐�����B
//�ЂƂ܂��͒��g�͋���ۂ����AInitScene�ň��Ăяo������
//�܂����ۂ̏������̓��e�͖��m��
void ezMap_init();

void ezMap_dataInit(ezMapDataT *data,int n, int m);
void ezMap_dataInit(int n, int m);

void ezMap_dataResize(ezMapDataT *data, int n, int m, bool isPreserve);
void ezMap_dataResize(int n, int m, bool isPreserve);

bool ezMap_castFromArray(ezMapDataT *mapData, int *data, int gridN);
bool ezMap_castFromArray(int *data, int gridN );

void ezMap_draw(void(*drawFunc)(int, ObjDataT*));

void ezMap_projectionDraw(int player_x, int player_z);

//�v���O�����I�����Ƀf�[�^�̌�n���B
//TermScene�̒��ň��ĂԁB
void ezMap_term();

int ezMap_getCellState(ezMapDataT const * data, int n, int m);
int ezMap_getCellState(int n, int m);
int ezMap_getCellState(ezMapDataT const * data, int i);
int ezMap_getCellState(int i);

void ezMap_setCellState(ezMapDataT * data, int n, int m, int state);
void ezMap_setCellState(int n, int m, int state);
void ezMap_setCellState(ezMapDataT * data, int i, int state);
void ezMap_setCellState(int i, int state);

ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int n, int m);
ObjDataT* ezMap_getCellObjData(int n, int m);
ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int i);
ObjDataT* ezMap_getCellObjData( int i);

ezMapDataT* ezMap_getMapData();
