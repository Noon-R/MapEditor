#include "platform.h"

#include "ezUtil.h"
#include "sim.h"

#include "mymodel.h" //★

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "ezMap.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

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


void drawMap(int a,ObjDataT* obj) {
	applyObjColor(obj);

	drawSolidCube();
}

/*---------------------------------------------------------------- DrawScene
 * DrawScene:
 *--------*/
void DrawScene( void )
{
	

	ezMap_draw(drawMap);
	
	glPushMatrix(); {
		applyObjTransform(&simdata.pointer);
		applyObjColor(&simdata.pointer);
		glScalef(simdata.pointer.radius, simdata.pointer.radius, simdata.pointer.radius);
		drawSolidSphere();
		//drawSolidCube();
	}
	glPopMatrix();

	
	

    return;
}

void DrawImGui() {

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();


	{
		ImGui::Begin("Test Window");

		ImGui::Text("Hello, world %d", 123);

		if (ImGui::Button("OK")) {
			printf("Button\n");
		}

		static char buf[256] = "aaa";
		if (ImGui::InputText("string", buf, 256)) {
			printf("InputText\n");
		}

		static float f = 0.0f;
		if (ImGui::SliderFloat("float", &f, 0.0f, 1.0f)) {
			printf("SliderFloat\n");
		}
		ImGui::End();
	}

	ImGui::Render();
}
