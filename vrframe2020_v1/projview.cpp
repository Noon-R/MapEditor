#include "platform.h"

#include "common.h"
#include "sim.h"

extern SimDataT simdata;
extern WindowDataT window;

/*---------------------------------------------------------------- Viewing
 * Viewing:
 *--------*/
void Viewing( void )
{
	//主観カメラの場合
    gluLookAt( 0.0, 1.0, 5.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 );
	
	if (simdata.active_camera == nullptr)simdata.active_camera = &simdata.player;

	glRotatef( - simdata.active_camera->rot.roll, 0.0, 0.0, 1.0 );
	glRotatef( - simdata.active_camera->rot.pitch, 1.0, 0.0, 0.0 );
	glRotatef( - simdata.active_camera->rot.yaw, 0.0, 1.0, 0.0 );
	//分かりやすいように改行していますが、改行なしでも構いません。
	glTranslatef( 	- simdata.active_camera->pos.x,
			- simdata.active_camera->pos.y,
			- simdata.active_camera->pos.z );

	//----------------------------------------------------
	//客観カメラ（固定カメラ）でプレイヤを注視する場合はこちら
	//gluLookAt( 0.0, 1.0, 3.0, simdata.player.x, simdata.player.y, simdata.player.z, 0.0, 1.0, 0.0 );
}
/*---------------------------------------------------------------- Projection
 * Projection:
 *--------*/
void Projection( void )
{
	//---- begin projection transformation

	glOrtho(-simdata.viewing * window.aspect, simdata.viewing* window.aspect, -simdata.viewing, simdata.viewing, simdata.clip_near, simdata.clip_far); //画面全体を-1.0～1.0の範囲に設定
    //gluPerspective( 45.0, window.aspect, simdata.clip_near, simdata.clip_far );
}
