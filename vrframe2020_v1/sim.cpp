#include "platform.h"

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "calc.h"
#include "sim.h"
#include "config.h"

#include "ezTrack.h"

#include "mymodel.h" //��

#include "ezMap.h"

SimDataT simdata; //SimDataT�^�\���̂̃f�[�^��錾
extern WindowDataT window;
extern MouseDataT mouse;
extern KeyDataT keydata; //���C����
extern int time; //�v���O�������N�������Ƃ��̎���
///////////////////////////////////////////////////////
//�g���b�J�[�f�o�C�X��L���ɂ���t���O
//bool use_tracker = false; //�L���ɂ���Ƃ�true

ezTracker *tracker; //���L�������o�R�Ńg���b�J�[�̏��𓾂�I�u�W�F�N�g
//�g���b�J�[����󂯎�����f�[�^�ւ̃|�C���^
ezTrackDataT *trackBase; //��}�[�J
ezTrackDataT *trackHead;
ezTrackDataT *trackBody;
ezTrackDataT *trackHandR;
ezTrackDataT *trackHandL;
ezTrackDataT *trackFootR;
ezTrackDataT *trackFootL;
//�}�[�J�������Ȃ��ꍇ�Ȃǂ̃_�~�[�f�[�^
ezTrackDataT localBase = { 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localHead = { 0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localBody = { 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
ezTrackDataT localHandR = { 0, 0.25, 1.25, -2.0, 0.0, 0.0, 0.0 };//��-2.0
ezTrackDataT localHandL = { 0,-0.25, 1.25, -2.0, 0.0, 0.0, 0.0 };//��-2.0
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
	////// �V�[���f�[�^�̏�����
	simdata.active_camera = &simdata.player;

	simdata.clip_far = 200.0; //���t�@�[�N���b�v�v���[��
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

	///���ǉ������I�u�W�F�N�g�̏�����

	setObjPos( &simdata.player, 0.0, 10.0, 0.0 );
	setObjRot( &simdata.player, 0.0, -90.0, 0.0 );
	setObjColor( &simdata.player, 0.0, 0.5, 1.0 );
	simdata.player.visible = true;
	simdata.player.state = 0;
	simdata.player.turn = 0.0;
	simdata.player.move = 0.0;
	simdata.player.radius = 0.5;

	simdata.active_camera = NULL;

	simdata.pointer = ObjDataT();
	simdata.pointer.pos.y = 0.25;
	simdata.pointer.radius = 0.25;
	setObjColor(&simdata.pointer,0.251,0.557,0.365);

	CreateMyModels(); //��

	

	return;
}
/*-------------------------------------------------------------- UpdateScene
 * UpdateScene:
 *--------*/
static float headRoll;
void UpdateScene(void)
{
	//////// �f�[�^�X�V ////////
	
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
			setObjColor(&data->cellObjs[i], 1, 0, 0);
			data->cells[i] = simdata.currentPaintNum;
		}

	}

	return;
}


