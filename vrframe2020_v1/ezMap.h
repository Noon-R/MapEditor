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
void ezMap_load(char const *file);

//メイズデータをセーブする
void ezMap_save(char const *file);

//ロードしたメイズデータをプリントで確認する
void ezMap_print();

//メイズデータを初期化する
//ロードされたメイズデータを元にして、描画用のデータを生成する。
//ひとまずは中身は空っぽだが、InitSceneで一回呼び出すこと
//まだ実際の初期化の内容は未確定
void ezMap_init();

void ezMap_dataInit(int n, int m);

void ezMap_draw(void(*drawFunc)(int, ObjDataT*));

void ezMap_projectionDraw(int player_x, int player_z);

//プログラム終了時にデータの後始末。
//TermSceneの中で一回呼ぶ。
void ezMap_term();

int ezMap_getCellState(ezMapDataT const * data, int n, int m);
int ezMap_getCellState(ezMapDataT const * data, int i);

int * ezMap_setCellState(ezMapDataT * data, int n, int m);
int * ezMap_setCellState(ezMapDataT * data, int i);

ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int n, int m);
ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int i);
