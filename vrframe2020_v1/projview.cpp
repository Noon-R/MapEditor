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
	//��σJ�����̏ꍇ
    gluLookAt( 0.0, 1.0, 5.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 );
	
	if (simdata.active_camera == nullptr)simdata.active_camera = &simdata.player;

	glRotatef( - simdata.active_camera->rot.roll, 0.0, 0.0, 1.0 );
	glRotatef( - simdata.active_camera->rot.pitch, 1.0, 0.0, 0.0 );
	glRotatef( - simdata.active_camera->rot.yaw, 0.0, 1.0, 0.0 );
	//������₷���悤�ɉ��s���Ă��܂����A���s�Ȃ��ł��\���܂���B
	glTranslatef( 	- simdata.active_camera->pos.x,
			- simdata.active_camera->pos.y,
			- simdata.active_camera->pos.z );

	//----------------------------------------------------
	//�q�σJ�����i�Œ�J�����j�Ńv���C���𒍎�����ꍇ�͂�����
	//gluLookAt( 0.0, 1.0, 3.0, simdata.player.x, simdata.player.y, simdata.player.z, 0.0, 1.0, 0.0 );
}
/*---------------------------------------------------------------- Projection
 * Projection:
 *--------*/
void Projection( void )
{
	//---- begin projection transformation

	glOrtho(-simdata.viewing * window.aspect, simdata.viewing* window.aspect, -simdata.viewing, simdata.viewing, simdata.clip_near, simdata.clip_far); //��ʑS�̂�-1.0�`1.0�͈̔͂ɐݒ�
    //gluPerspective( 45.0, window.aspect, simdata.clip_near, simdata.clip_far );
}
