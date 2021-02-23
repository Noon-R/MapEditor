#include "PostDraw.h"

#include "platform.h"

#include "ezUtil.h"
#include "sim.h"

extern SimDataT simdata;

void PostDraw(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//2Dの投影変換を設定する（平衡投影）
	glMatrixMode(GL_PROJECTION);//投影変換マトリクス編集モード

	glLoadIdentity();//初期化

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); //画面全体を-1.0～1.0の範囲に設定

	glMatrixMode(GL_MODELVIEW);//モデリング変換マトリクスモード

	glLoadIdentity();//初期化

	/////////////////////////////////////////////

	DrawHUD();
	

	glFlush();
	glEnable(GL_DEPTH_TEST); // ---- begin: 
	glEnable(GL_LIGHTING);

}

void drawString(float x, float y, void *font, char *string)
{
	//* 現在のラスターポジションの保存 */
	glPushAttrib(GL_CURRENT_BIT);

	glRasterPos2f(x, y);

	//* ビットマップ文字列の描画 */
	while (*string) {
		glutBitmapCharacter(font, *string++);
	}
	//* 保存したラスターポジションのロード */
	glPopAttrib();
	return;
}

void DrawHUD()
{
	/*数値の文字列変換
	char roll[30];
	sprintf(roll, "rot : %f", 1324);
	*/

	/*
	glColor3f(1.0, 1.0, 0.0); //文字色

	drawString(-1,-0.98,
		GLUT_BITMAP_TIMES_ROMAN_24,
		"(-1,-1)");

	drawString(-1, 0.92,
		GLUT_BITMAP_TIMES_ROMAN_24,
		"(-1, 1)");

	drawString(0.91, -0.98,
		GLUT_BITMAP_TIMES_ROMAN_24,
		"( 1,-1)");

	drawString(0.91, 0.92,
		GLUT_BITMAP_TIMES_ROMAN_24,
		"( 1, 1)");

	*/
	
}
