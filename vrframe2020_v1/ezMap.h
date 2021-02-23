#pragma once
#include <vector>

#include "object.h"

#define CELL_SIZE 1

typedef struct {

	int field_width, field_height; //メイズフィールド内の縦横、升目の個数
	std::vector<int> cells;
	std::vector<ObjDataT> cellObjs;

} ezMapDataT;

//メイズデータをロードする
//keyboardコールバック関数で、例えば[L]キーでデータをロードする
bool ezMap_load(char const *file);

//メイズデータをセーブする
bool ezMap_save(char const *file);

//ロードしたメイズデータをプリントで確認する
void ezMap_print();

//メイズデータを初期化する
//ロードされたメイズデータを元にして、描画用のデータを生成する。
//ひとまずは中身は空っぽだが、InitSceneで一回呼び出すこと
//まだ実際の初期化の内容は未確定
void ezMap_init();

void ezMap_dataInit(ezMapDataT *data,int n, int m);
void ezMap_dataInit(int n, int m);

void ezMap_dataResize(ezMapDataT *data, int n, int m, bool isPreserve);
void ezMap_dataResize(int n, int m, bool isPreserve);

bool ezMap_castFromArray(ezMapDataT *mapData, int *data, int gridN);
bool ezMap_castFromArray(int *data, int gridN );

void ezMap_draw(void(*drawFunc)(int, ObjDataT*));

void ezMap_projectionDraw(int player_x, int player_z);

//プログラム終了時にデータの後始末。
//TermSceneの中で一回呼ぶ。
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
