#include "platform.h"

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "calc.h"
#include "sim.h"
#include "config.h"

#include "ezTrack.h"

#include "mymodel.h" //★

#include "ezMap.h"


SimDataT simdata; //SimDataT型構造体のデータを宣言
extern WindowDataT window;
extern MouseDataT mouse;
extern KeyDataT keydata; //★修正★
extern int time; //プログラムが起動したときの時間



/*--------------------
 * copyTrackToObj
 */
static void copyTrackToObj( ezTrackDataT *src, ObjDataT *dst )
{
	float pos[3], rot[3];
	if( src == NULL ) return;
	ezTrack_getPos( src, pos );
	setObjPos( dst, pos );
	ezTrack_getRot( src, rot );
	setObjRot( dst, rot );
}


/*---------------------------------------------------------------- InitScene
 * InitScene:
 *--------*/
void InitScene( void )
{
	ezMap_load("tempmap.txt");
	////// シーンデータの初期化
	simdata.active_camera = &simdata.player;


	simdata.clip_far = 200.0; //◆ファークリッププレーン
	simdata.clip_near = 0.1;
	simdata.air_color[0] = 1.0;
	simdata.air_color[1] = 1.0;
	simdata.air_color[2] = 1.0;
	simdata.air_color[3] = 0.1; // fog density factor
	simdata.sky_color[0] = 0.0;
	simdata.sky_color[1] = 0.0;
	simdata.sky_color[2] = 0.0;
	simdata.sky_color[3] = 0.8; // sky color factor
	//////

	///▼追加したオブジェクトの初期化

	setObjPos(&simdata.player, 0.0, 10.0, 0.0);
	setObjRot(&simdata.player, 0.0, -90.0, 0.0);
	setObjColor(&simdata.player, 0.0, 0.5, 1.0);

	simdata.player.visible = true;
	simdata.player.state = 0;
	simdata.player.turn = 0.0;
	simdata.player.move = 0.0;
	simdata.player.radius = 0.5;

	simdata.pointer = ObjDataT();
	simdata.pointer.pos.y = 0.25;
	simdata.pointer.radius = 0.25;
	simdata.pointer.state = 0;
	setObjColor(&simdata.pointer,0.251,0.557,0.365);

	CreateMyModels(); //★

	

	return;
}
/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
static float headRoll;
void UpdateScene(void)
{
	//////// データ更新 ////////
	
	simdata.player.pos.z -= keydata.charKey['w'] ? 0.3 : 0;
	simdata.player.pos.z += keydata.charKey['s'] ? 0.3 : 0;
	simdata.player.pos.x -= keydata.charKey['a'] ? 0.3 : 0;
	simdata.player.pos.x += keydata.charKey['d'] ? 0.3 : 0;

	simdata.viewing += keydata.charKey['k'] ? 0.3 : 0;
	simdata.viewing -= keydata.charKey['l'] ? 0.3 : 0;


	simdata.pointer.pos.x = mouse.xAbs * simdata.viewing * window.aspect;
	simdata.pointer.pos.z = mouse.yAbs * simdata.viewing;
	simdata.pointer.pos.z -= 1.0;
	simdata.pointer.pos.x += simdata.player.pos.x;
	simdata.pointer.pos.z += simdata.player.pos.z;

	simdata.pointer.state = mouse.left;

	ObjDataT test;
	copyObj(&simdata.pointer,&test);
	moveLocalToWorld(&test);
	printf("%f\n",test.pos.y);

	ezMapDataT* data = ezMap_getMapData();

	for (int i = 0; i < data->field_height * data->field_width; i++) {
		if (isHitBox(&data->cellObjs[i], &simdata.pointer) && simdata.pointer.state != 0) {
			setObjColor(&data->cellObjs[i], 
				simdata.currentColor.red,
				simdata.currentColor.green,
				simdata.currentColor.blue);
			data->cells[i] = simdata.currentPaintNum;
		}

	}

	return;
}


