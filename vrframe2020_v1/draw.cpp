#include "platform.h"

#include "ezUtil.h"
#include "sim.h"

#include "mymodel.h" //��

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
		glTranslatef( 0.0, 0.0, 0.0 );   //�I�u�W�F�N�g��ʒu����
		glRotatef( 0.0, 0.0, 1.0, 0.0 ); //�I�u�W�F�N�g��p�������F���[�p
		glRotatef( 0.0, 1.0, 0.0, 0.0 ); //�I�u�W�F�N�g��p�������F�s�b�`�p
		glRotatef( 0.0, 0.0, 0.0, 1.0 ); //�I�u�W�F�N�g��p�������F���[���p
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
		glTranslatef( 0.0, 0.0, 0.0 );    //�I�u�W�F�N�g��ʒu����
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //�I�u�W�F�N�g��p�������F���[�p
		glRotatef( 90.0, 1.0, 0.0, 0.0 ); //�I�u�W�F�N�g��p�������F�s�b�`�p
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //�I�u�W�F�N�g��p�������F���[���p
		glutSolidSphere( 0.5, 18, 16 );   //���a�C�o�x�����������C�ܓx����������
	}
	glPopMatrix();
    return;
}



//----------------------------------------------- drawPlayer
void drawPlayer( void )
{
	glPushMatrix();
	{
		glTranslatef( 0.0, 0.0, 0.0 );    //�I�u�W�F�N�g��ʒu����
		glRotatef( 0.0, 0.0, 1.0, 0.0 );  //�I�u�W�F�N�g��p�������F���[�p
		glRotatef( 180.0, 1.0, 0.0, 0.0 ); //�I�u�W�F�N�g��p�������F�s�b�`�p
		glRotatef( 0.0, 0.0, 0.0, 1.0 );  //�I�u�W�F�N�g��p�������F���[���p
		glutSolidCone( 0.5, 1.0, 6, 4 );   //���a�C�o�x�����������C�ܓx����������
	}
	glPopMatrix();
}

void drawHandR( void )
{
	glPushMatrix();
	applyObjColor( &simdata.handR );
	applyObjTransform( &simdata.handR );
	glutSolidSphere( 0.1, 8, 6 );
	//drawHandRmodel();//mymodels.cpp�ō�����`��֐�
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


	////// �v���C����`�悷��
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
