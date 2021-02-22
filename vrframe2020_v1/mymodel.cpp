#include "platform.h"
#include "GLMetaseq.h"

#include "sim.h"
#include "mymodel.h"

//★追加するモデル用の変数を追加する

static MQO_MODEL sample;

static GLuint globj_block;
static GLuint globj_stars;

void applyMaterialColor( float r, float g, float b );

/////////////////////////// 共通関数
/*----------------------------------------------------- CreateMyModels
 * CreateMyModels:InitSceneで一回だけ呼び出すこと
 */
void CreateMyModels()
{

	//▼モデルファイル読み込み
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
/////////////////////////// 個別の関数

void drawSampleModel()
{
    glPushMatrix();
    //glRotatef( 90.0, 1.0, 0.0, 0.0 );//モデルの座標軸調整など
    mqoCallModel( sample );
    glPopMatrix();
}

