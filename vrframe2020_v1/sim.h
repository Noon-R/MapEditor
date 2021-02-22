#ifndef __SIM_H__
#define __SIM_H__

#include "platform.h"
#include "calc.h"
#include "object.h"
#include <vector>

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
★ObjDataTの定義は、object.hに移行しました
*/
//-------- simulation data
typedef struct {

	float clip_near;
	float clip_far;
	float air_color[4];
	float sky_color[4];
    //////// ユーザ定義のシーンについてのデータをここにまとめる

	ObjDataT player; /////◆プレイヤ（の足元★）をあらわすオブジェクト
	ObjDataT head;

	ObjDataT pointer;

	ObjDataT *active_camera; //カメラのポインタ変数

	float viewing = 20;
	int currentPaintNum = 1;

	std::vector<color_t> paintCols;

	////////
} SimDataT;

#endif //__SIM_H__
