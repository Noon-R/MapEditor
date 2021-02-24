#include "ezMap.h"

#include <stdio.h>
#include <string>
#include "platform.h"

#include "calc.h"

ezMapDataT map;

char rootPath[] = "mapData/";

//メイズデータをロードする
//keyboardコールバック関数で、例えば[L]キーでデータをロードする
bool ezMap_load(char const *file)
{

	FILE *fp;
	std::string path = rootPath + std::string(file);
	int n, m;

	fp = fopen(path.c_str(), "r");

	if (fp == NULL) {
		ezMap_dataInit(32, 32);
		return false;
	}
	//read M, N
	fscanf(fp, "%d,%d", &n, &m);

	ezMap_dataInit(n, m);

	for (int i = 0; i < map.field_height; i++) {
		for (int j = 0; j < map.field_width; j++) {
			int state = 0;
			fscanf(fp, "%d ", &state);
			ezMap_setCellState(&map, i, j, state);
		}
	}
	fclose(fp);
	return true;
}

void ezMap_print()
{
	printf("%d,%d\n", map.field_width, map.field_height);
	for (int i = 0; i < map.field_height; i++) {
		for (int j = 0; j < map.field_width; j++) {
			printf("%d ", ezMap_getCellState(&map, i, j));
		}
		printf("\n");
	}
}

//メイズデータをセーブする
bool ezMap_save(char const *file)
{
	char fileName[32];
	if (strcmp(file, "") == 0) {
		strcpy(fileName, "mapData.txt");
	}
	else {
		strcpy(fileName, file);
	}

	FILE *fp;
	std::string path = rootPath + std::string(fileName);
	fp = fopen(path.c_str(), "w");

	if (fp == NULL)return false;

	//read M, N
	fprintf(fp, "%d,%d\n", map.field_width, map.field_height);
	for (int i = 0; i < map.field_height; i++) {
		for (int j = 0; j < map.field_width; j++) {
			fprintf(fp, "%d ", ezMap_getCellState(&map, i, j));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return true;
}

//メイズデータを初期化する
//ロードされたメイズデータを元にして、描画用のデータを生成する。
//ひとまずは中身は空っぽだが、InitSceneで一回呼び出すこと
//まだ実際の初期化の内容は未確定
void ezMap_init()
{
	ezMap_dataInit(1,1);
	return;
}

void ezMap_dataInit(ezMapDataT * data, int n, int m)
{
	{
		std::vector<int> swapCell;
		std::vector<ObjDataT> swapObj;

		map.cells.swap(swapCell);
		map.cellObjs.swap(swapObj);
	}

	data->field_width = n;
	data->field_height = m;
	data->cells.resize(n*m, 0);
	data->cellObjs.resize(n*m, ObjDataT());
}

void ezMap_dataInit(int n, int m) {
	ezMap_dataInit(&map,n,m);
}

void ezMap_dataResize(ezMapDataT *data, int n, int m, bool isPreserve) {
	ezMapDataT oldData = *data;

	ezMap_dataInit(data, n, m);
	if (isPreserve) {
	
		for (int x = 0; x < oldData.field_width; x++) {
			if (x >= data->field_width) continue;
			for (int y = 0; y < oldData.field_height; y++) {
				if (y >= data->field_height) continue;

				ezMap_setCellState(data, 
					x,
					y,
					ezMap_getCellState(&oldData, x, y));
				
				*ezMap_getCellObjData(data, x,y) = *ezMap_getCellObjData(&oldData, x, y);

			}
		}
	}
}

void ezMap_dataResize(int n, int m, bool isPreserve)
{
	ezMap_dataResize(&map,n,m,isPreserve);
}

bool ezMap_castFromArray(ezMapDataT *mapData, int *data, int gridN)
{
	ezMap_dataInit(mapData,gridN, gridN);
	for (int x = 0; x < gridN; x++) {
		for (int y = 0; y < gridN; y++) {
			ezMap_setCellState(mapData, x,y, data[y+ x*gridN]);
		}
	}
	return true;
}

bool ezMap_castFromArray(int * data, int gridN)
{
	return ezMap_castFromArray(&map,data,gridN);
}

void ezMap_draw(void(*drawFunc)(int, ObjDataT*))
{
	float range = map.field_width*1.1; //マップの一辺の長さ（マップの広さ）
	int i, j; //ループ制御変数
	float x, y, z; //位置座標用変数
	for (i = 0; i < map.field_width; i++) {
		z = range * ((float)i / map.field_width - 0.5); //(i,j)座標から(z,x)座標への変換
		for (j = 0; j < map.field_height; j++) {
			x = range * ((float)j / map.field_height - 0.5); //(i,j)座標から(z,x)座標への変換
			y = -1.5;
			glPushMatrix(); {
				glTranslatef(x, y, z);
				setObjPos(&map.cellObjs[i+j*map.field_width],x,0,z);
				drawFunc(
					ezMap_getCellState(&map, i, j),
					ezMap_getCellObjData(&map, i, j));
			}
			glPopMatrix();
		}
	}

	return;
}

void ezMap_projectionDraw(int player_x, int player_z) {
	int m, n;
	float xoffset = (float)map.field_width * CELL_SIZE / 2.0;
	float yoffset = (float)map.field_height* CELL_SIZE / 2.0;
	float x, y, z;
	y = 0.0;

	for (n = 0; n < map.field_height; n++) {
		z = ((float)n + 0.5) * CELL_SIZE - yoffset;
		for (m = 0; m < map.field_width; m++) {
			x = ((float)m + 0.5) * CELL_SIZE - xoffset;

			if (ezMap_getCellState(&map, n, m) > 0) glColor3f(ezMap_getCellState(&map, n, m) * 0.125, 0.0, 1.0 - 0.125 * ezMap_getCellState(&map, n, m));
			else glColor3f(0.0, 0.5, 1.0);

			glPushMatrix();
			if (player_x == m && player_z == n) {
				glLineWidth(2.0);
				glColor3f(1.0, 0.0, 0.0);
				glTranslatef(x, y + 0.1, z);
			}
			else {
				glLineWidth(1.0);
				glTranslatef(x, y, z);
			}
			glRotatef(0.0, 0.0, 1.0, 0.0);
			glRotatef(0.0, 1.0, 0.0, 0.0);
			glRotatef(0.0, 0.0, 0.0, 1.0);
			//drawCube(maze.cell[n][m]);
			glPopMatrix();
		}
	}
}

//プログラム終了時にデータの後始末。
//TermSceneの中で一回呼ぶ。
void ezMap_term()
{
	return;
}


int ezMap_getCellState(ezMapDataT const *data, int n, int m) {
	return ezMap_getCellState(data, n + m * data->field_width);
}

int ezMap_getCellState(int n, int m)
{
	return ezMap_getCellState(&map, n + m * map.field_width);
}

int ezMap_getCellState(ezMapDataT const * data, int i)
{
	return data->cells[i];
}

int ezMap_getCellState(int i)
{
	return ezMap_getCellState(&map,i);
}

void ezMap_setCellState(ezMapDataT *data, int n, int m, int state) {
	ezMap_setCellState(data, n + m * data->field_width, state);
}

void ezMap_setCellState(int n, int m, int state)
{
	ezMap_setCellState(&map, n,m, state);
}

void ezMap_setCellState(ezMapDataT * data, int i, int state)
{
	data->cells[i] = state;
}

void ezMap_setCellState(int i, int state)
{
	ezMap_setCellState(&map,i,state);
}

ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int n, int m) {
	return ezMap_getCellObjData(data, n + m * data->field_width);
}

ObjDataT * ezMap_getCellObjData(int n, int m)
{
	return ezMap_getCellObjData(&map,n,m);
}

ObjDataT * ezMap_getCellObjData(ezMapDataT * data, int i)
{
	return &data->cellObjs[i];

}

ObjDataT * ezMap_getCellObjData(int i)
{
	return ezMap_getCellObjData(&map,i);
}

ezMapDataT * ezMap_getMapData() {
	return &map;
}
