#include "platform.h"

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "calc.h"
#include "sim.h"
#include "config.h"

#include "ezTrack.h"

#include "mymodel.h" //★

#include "ezMap.h"

SimDataT simdata; //SimDataT型構造体のデータを宣言
extern MouseDataT mouse;
extern KeyDataT keydata; //★修正★
extern int time; //プログラムが起動したときの時間
///////////////////////////////////////////////////////
//トラッカーデバイスを有効にするフラグ
//bool use_tracker = false; //有効にするときtrue

ezTracker *tracker; //共有メモリ経由でトラッカーの情報を得るオブジェクト
//トラッカーから受け取ったデータへのポインタ
ezTrackDataT *trackBase; //基準マーカ
ezTrackDataT *trackHead;
ezTrackDataT *trackBody;
ezTrackDataT *trackHandR;
ezTrackDataT *trackHandL;
ezTrackDataT *trackFootR;
ezTrackDataT *trackFootL;
//マーカが見えない場合などのダミーデータ
ezTrackDataT localBase = { 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localHead = { 0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localBody = { 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localHandR = { 0, 0.25, 1.25, -2.0, 0.0, 0.0, 0.0 };//★-2.0
ezTrackDataT localHandL = { 0,-0.25, 1.25, -2.0, 0.0, 0.0, 0.0 };//★-2.0
ezTrackDataT localFootR = { 0, 0.25, 0.0, -0.5, 0.0, 0.0, 0.0 };
ezTrackDataT localFootL = { 0,-0.25, 0.0, -0.5, 0.0, 0.0, 0.0 };
////////////////////////////////////////////////////////


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
	
	simdata.handR.radius = 0.25;//◆04
	simdata.handL.radius = 0.25;//◆04

	setObjPos( &simdata.player, 0.0, 10.0, 0.0 );
	setObjRot( &simdata.player, 0.0, -90.0, 0.0 );
	setObjColor( &simdata.player, 0.0, 0.5, 1.0 );
	simdata.player.visible = true;
	simdata.player.state = 0;
	simdata.player.turn = 0.0;
	simdata.player.move = 0.0;
	simdata.player.radius = 0.5;


	//右手（ローカル座標）をプレイヤの子座標系とする
	setObjLocal( &simdata.handR, &simdata.player ); //★
	//PlayerにHandRをぶら下げる

	//★左手も同様
	setObjLocal( &simdata.handL, &simdata.player ); //★

	//頭をプレイヤーの子座標系にする
	setObjLocal( &simdata.head, &simdata.player );

	//simdata.active_camera = &simdata.player;
	simdata.active_camera = NULL;
	//プレイヤオブジェクトのアドレスをカメラのポインタに紐付ける

	setObjColor( &simdata.handR, 0.0, 1.0, 0.0 ); //右手グリーン
	setObjColor( &simdata.handL, 1.0, 0.0, 0.0 ); //左手レッド

	tracker = new ezTracker( use_tracker ); //VICON使うときはtrue
	tracker->open( "VICON", false ); //識別名, Wフラグ(false:R/O)

	trackHead = &localHead;
	trackBody = &localBody;
    trackHandR = &localHandR;
	trackHandL = &localHandL;
	trackBase = &localBase;
	trackFootL = &localFootL;
	trackFootR = &localFootR;

	//★修正★UpdataSceneからコピー
	copyTrackToObj( trackHead, &simdata.head );
    copyTrackToObj( trackBody, &simdata.body );
	copyTrackToObj( trackHandL, &simdata.handL );
	copyTrackToObj( trackHandR, &simdata.handR );
	copyTrackToObj( trackFootL, &simdata.footL );
	copyTrackToObj( trackFootR, &simdata.footR );

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
	if (use_tracker) {
		//トラッカーからのデータをゲット
		tracker->read(); //共有メモリからデータを読み出す
		trackHead = tracker->getTrackData("ShutterGlasses"); //VICONマーカの名前
		trackBody = tracker->getTrackData("Chest");
		trackHandR = tracker->getTrackData("TREE_A");
		trackHandL = tracker->getTrackData("TREE_B");
		trackHead = tracker->getTrackData("CAP");
		trackBase = tracker->getTrackData("Candy");
		trackFootR = tracker->getTrackData("RightFoot");
		trackFootL = tracker->getTrackData("LeftFoot");

		copyTrackToObj(trackHead, &simdata.head);
		copyTrackToObj(trackBody, &simdata.body);
		copyTrackToObj(trackHandL, &simdata.handL);
		copyTrackToObj(trackHandR, &simdata.handR);
		copyTrackToObj(trackFootL, &simdata.footL);
		copyTrackToObj(trackFootR, &simdata.footR);

		simdata.handL.pos.z -= 1.2;
		simdata.handR.pos.z -= 1.2;

		simdata.player.pos.x = simdata.head.pos.x * 2;

	} else {
		simdata.player.pos.z -= keydata.charKey['w'] ? 0.3 : 0;
		simdata.player.pos.z += keydata.charKey['s'] ? 0.3 : 0;
		simdata.player.pos.x -= keydata.charKey['a'] ? 0.3 : 0;
		simdata.player.pos.x += keydata.charKey['d'] ? 0.3 : 0;
		
		simdata.viewing += keydata.charKey['k'] ? 0.3 : 0;
		simdata.viewing -= keydata.charKey['l'] ? 0.3 : 0;

	}


	return;
}


