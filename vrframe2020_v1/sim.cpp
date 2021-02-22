#include "platform.h"

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "calc.h"
#include "sim.h"
#include "config.h"

#include "ezTrack.h"

#include "mymodel.h" //��

#include "ezMap.h"

SimDataT simdata; //SimDataT�^�\���̂̃f�[�^��錾
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
	
	simdata.handR.radius = 0.25;//��04
	simdata.handL.radius = 0.25;//��04

	setObjPos( &simdata.player, 0.0, 10.0, 0.0 );
	setObjRot( &simdata.player, 0.0, -90.0, 0.0 );
	setObjColor( &simdata.player, 0.0, 0.5, 1.0 );
	simdata.player.visible = true;
	simdata.player.state = 0;
	simdata.player.turn = 0.0;
	simdata.player.move = 0.0;
	simdata.player.radius = 0.5;


	//�E��i���[�J�����W�j���v���C���̎q���W�n�Ƃ���
	setObjLocal( &simdata.handR, &simdata.player ); //��
	//Player��HandR���Ԃ牺����

	//����������l
	setObjLocal( &simdata.handL, &simdata.player ); //��

	//�����v���C���[�̎q���W�n�ɂ���
	setObjLocal( &simdata.head, &simdata.player );

	//simdata.active_camera = &simdata.player;
	simdata.active_camera = NULL;
	//�v���C���I�u�W�F�N�g�̃A�h���X���J�����̃|�C���^�ɕR�t����

	setObjColor( &simdata.handR, 0.0, 1.0, 0.0 ); //�E��O���[��
	setObjColor( &simdata.handL, 1.0, 0.0, 0.0 ); //���背�b�h

	tracker = new ezTracker( use_tracker ); //VICON�g���Ƃ���true
	tracker->open( "VICON", false ); //���ʖ�, W�t���O(false:R/O)

	trackHead = &localHead;
	trackBody = &localBody;
    trackHandR = &localHandR;
	trackHandL = &localHandL;
	trackBase = &localBase;
	trackFootL = &localFootL;
	trackFootR = &localFootR;

	//���C����UpdataScene����R�s�[
	copyTrackToObj( trackHead, &simdata.head );
    copyTrackToObj( trackBody, &simdata.body );
	copyTrackToObj( trackHandL, &simdata.handL );
	copyTrackToObj( trackHandR, &simdata.handR );
	copyTrackToObj( trackFootL, &simdata.footL );
	copyTrackToObj( trackFootR, &simdata.footR );

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
	if (use_tracker) {
		//�g���b�J�[����̃f�[�^���Q�b�g
		tracker->read(); //���L����������f�[�^��ǂݏo��
		trackHead = tracker->getTrackData("ShutterGlasses"); //VICON�}�[�J�̖��O
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


