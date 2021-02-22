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
void ezMap_load(char const *file);

//���C�Y�f�[�^���Z�[�u����
void ezMap_save(char const *file);

//���[�h�������C�Y�f�[�^���v�����g�Ŋm�F����
void ezMap_print();

//���C�Y�f�[�^������������
//���[�h���ꂽ���C�Y�f�[�^�����ɂ��āA�`��p�̃f�[�^�𐶐�����B
//�ЂƂ܂��͒��g�͋���ۂ����AInitScene�ň��Ăяo������
//�܂����ۂ̏������̓��e�͖��m��
void ezMap_init();

void ezMap_dataInit(int n, int m);

void ezMap_draw(void(*drawFunc)(int, ObjDataT*));

void ezMap_projectionDraw(int player_x, int player_z);

//�v���O�����I�����Ƀf�[�^�̌�n���B
//TermScene�̒��ň��ĂԁB
void ezMap_term();

int ezMap_getCellState(ezMapDataT const * data, int n, int m);
int ezMap_getCellState(ezMapDataT const * data, int i);

int * ezMap_setCellState(ezMapDataT * data, int n, int m);
int * ezMap_setCellState(ezMapDataT * data, int i);

ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int n, int m);
ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int i);
