#include "platform.h"
#include "GLMetaseq.h"

#include "sim.h"
#include "mymodel.h"

//���ǉ����郂�f���p�̕ϐ���ǉ�����

static MQO_MODEL sample;

static GLuint globj_block;
static GLuint globj_stars;

void applyMaterialColor( float r, float g, float b );

/////////////////////////// ���ʊ֐�
/*----------------------------------------------------- CreateMyModels
 * CreateMyModels:InitScene�ň�񂾂��Ăяo������
 */
void CreateMyModels()
{

	//�����f���t�@�C���ǂݍ���
	//chicken = mqoCreateModel( "data/full_body_chicken.mqo", 0.1 );

	sample = mqoCreateModel( "data/kumanomi.mqo", 0.001 );

}

/*----------------------------------------------------- DeleteMyModels
 * DeleteMyModels
 */
void DeleteMyModels()
{
    mqoDeleteModel(sample);
}
/////////////////////////// �ʂ̊֐�

void drawSampleModel()
{
    glPushMatrix();
    //glRotatef( 90.0, 1.0, 0.0, 0.0 );//���f���̍��W�������Ȃ�
    mqoCallModel( sample );
    glPopMatrix();
}

