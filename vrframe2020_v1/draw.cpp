#include "platform.h"

#include "ezUtil.h"
#include "sim.h"

#include "mymodel.h" //★

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "ezMap.h"

extern WindowDataT window;
extern SimDataT simdata;


/*----------------------------------------------------------- drawSolidCube
 * drawSolidCube:
 *--------*/
void drawSolidCube( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );   //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 ); //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 ); //オブジェクト基準姿勢調整：ロール角
		glutSolidCube( 1.0 );
	}
	glPopMatrix();
    return;
}
/*--------------------------------------------------------- drawSolidSphere
 * drawSolidSphere:
 *--------*/
void drawSolidSphere( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 90.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		glutSolidSphere( 0.5, 18, 16 );   //半径，経度方向分割数，緯度方向分割数
	}
	glPopMatrix();
    return;
}



//----------------------------------------------- drawPlayer
void drawPlayer( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );    //オブジェクト基準位置調整
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //オブジェクト基準姿勢調整：ヨー角
		glRotatef( 180.0, 1.0, 0.0, 0.0 ); //オブジェクト基準姿勢調整：ピッチ角
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //オブジェクト基準姿勢調整：ロール角
		glutSolidCone( 0.5, 1.0, 6, 4 );   //半径，経度方向分割数，緯度方向分割数
	}
	glPopMatrix();
}

void drawHandR( void )
{
	glPushMatrix();
	applyObjColor( &simdata.handR );
	applyObjTransform( &simdata.handR );
	glutSolidSphere( 0.1, 8, 6 );
	//drawHandRmodel();//mymodels.cppで作った描画関数
	glPopMatrix();
}
void drawHandL( void )
{
	glPushMatrix();
	applyObjColor( &simdata.handL );
	applyObjTransform( &simdata.handL );
	glutSolidSphere( 0.1, 8, 6 );
	//drawHandLmodel();
	glPopMatrix();
}

void drawMap(int a) {
	if (a != 0) {
		applyMaterialColor(1,0,0);
	}
	else {
		applyMaterialColor(1, 1, 1);

	}

	drawSolidCube();
}

/*---------------------------------------------------------------- DrawScene
 * DrawScene:
 *--------*/
void DrawScene( void )
{
	

	///////////////////////////////////////////////
	////ezUtil_drawGrid( 100, 100.0 );

	//glPushMatrix();
	////applyObjTransform( &simdata.player );
	//drawHandL();
	//drawHandR();
	//glPopMatrix();


	////// プレイヤを描画する
	//glPushMatrix();
	//{
	//	applyObjTransform( &simdata.player );
	//	applyObjColor( &simdata.player );
	//	//drawPlayer();
	//}
	//glPopMatrix();

	ezMap_draw(drawMap);
	
    return;
}
