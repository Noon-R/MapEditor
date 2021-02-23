#include "PostDraw.h"

#include "platform.h"

#include "ezUtil.h"
#include "sim.h"

extern SimDataT simdata;

void PostDraw(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//2D�̓��e�ϊ���ݒ肷��i���t���e�j
	glMatrixMode(GL_PROJECTION);//���e�ϊ��}�g���N�X�ҏW���[�h

	glLoadIdentity();//������

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); //��ʑS�̂�-1.0�`1.0�͈̔͂ɐݒ�

	glMatrixMode(GL_MODELVIEW);//���f�����O�ϊ��}�g���N�X���[�h

	glLoadIdentity();//������

	/////////////////////////////////////////////

	DrawHUD();
	

	glFlush();
	glEnable(GL_DEPTH_TEST); // ---- begin: 
	glEnable(GL_LIGHTING);

}

void drawString(float x, float y, void *font, char *string)
{
	//* ���݂̃��X�^�[�|�W�V�����̕ۑ� */
	glPushAttrib(GL_CURRENT_BIT);

	glRasterPos2f(x, y);

	//* �r�b�g�}�b�v������̕`�� */
	while (*string) {
		glutBitmapCharacter(font, *string++);
	}
	//* �ۑ��������X�^�[�|�W�V�����̃��[�h */
	glPopAttrib();
	return;
}

void DrawHUD()
{
	/*���l�̕�����ϊ�
	char roll[30];
	sprintf(roll, "rot : %f", 1324);
	*/

	/*
	glColor3f(1.0, 1.0, 0.0); //�����F

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
