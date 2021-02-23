#include "ezMap.h"

#include <stdio.h>
#include <string>
#include "platform.h"

#include "calc.h"

ezMapDataT map;

char rootPath[] = "mapData/";

//メイズデータをロードする
//keyboardコールバック関数で、例えば[L]キーでデータをロードする
void ezMap_load(char const *file)
{

	FILE *fp;
	std::string path = rootPath + std::string(file);
	int n, m;

	fp = fopen(path.c_str(), "r");
	//read M, N
	fscanf(fp, "%d,%d", &n, &m);

	ezMap_dataInit(n, m);

	for (int i = 0; i < map.field_height; i++) {
		for (int j = 0; j < map.field_width; j++) {
			fscanf(fp, "%d ", ezMap_setCellState(&map, i, j));
		}
	}
	fclose(fp);
	return;
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
void ezMap_save(char const *file)
{
	char fileName[32];
	if (file == "") strcpy(fileName, "mapData.txt");
	strcpy(fileName, file);

	FILE *fp;
	std::string path = rootPath + std::string(fileName);
	fp = fopen(path.c_str(), "w");
	//read M, N
	fprintf(fp, "%d,%d\n", map.field_width, map.field_height);
	for (int i = 0; i < map.field_height; i++) {
		for (int j = 0; j < map.field_width; j++) {
			fprintf(fp, "%d ", ezMap_getCellState(&map, i, j));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return;
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

void ezMap_dataInit(int n, int m) {
	{
		std::vector<int> swapCell;
		std::vector<ObjDataT> swapObj;

		map.cells.swap(swapCell);
		map.cellObjs.swap(swapObj);
	}

	map.field_width  = n;
	map.field_height = m;
	map.cells.resize(n*m, 0);
	map.cellObjs.resize(n*m, ObjDataT());
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

int ezMap_getCellState(ezMapDataT const * data, int i)
{
	return data->cells[i];
}

int * ezMap_setCellState(ezMapDataT *data, int n, int m) {
	return ezMap_setCellState(data, n + m * data->field_width);
}

int * ezMap_setCellState(ezMapDataT * data, int i)
{
	return &data->cells[i];
}

ObjDataT* ezMap_getCellObjData(ezMapDataT *data, int n, int m) {
	return ezMap_getCellObjData(data, n + m * data->field_width);
}

ObjDataT * ezMap_getCellObjData(ezMapDataT * data, int i)
{
	return &data->cellObjs[i];

}

ezMapDataT * ezMap_getMapData() {
	return &map;
}
