/******************************************************************************
 * glsample_frame.cpp 
 * Level 3 - base framework */

#include "platform.h"

#include "common.h" //WindowDataT, MouseDataT, KeyDataT
#include "sim.h"
#include "config.h"

#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

//#include "PostDraw.h"
/*
const bool cylindrical = true; //◆シリンドリカルフラグ
const bool fullscreen = true; //◆フルスクリーンフラグ
const bool stereo = false;    //◆ステレオフラグ
*/

//-------- callback functions
void display( void );
static void update( void );
static void reshape( int width, int height );

//-------- init GL window
static void initWindow( char *title );

//-------- simple view
void singleView( float dx );

// projview.cpp
void Viewing( void );
void Projection( void );

//-------- mouse/kbdmouse.cpp: mouse callback functions
void mouseClick( int button , int state, int x, int y );
void mouseDrag( int x, int y );
void mouseMotion( int x, int y );

//-------- keyboard/kbdmouse.cpp: keyboard callback functions
void charKeyDown( unsigned char key, int x, int y );
void charKeyUp( unsigned char key, int x, int y );
void funcKeyDown( int key, int x, int y );
void funcKeyUp( int key, int x, int y );

WindowDataT window;
extern SimDataT simdata;


// cylindrical.cpp
void cylindricalView( float dx );

//stereo.cpp
void Stereo( void );

//-------- light.cpp
void MainLight( int light_id, float r, float g, float b );
void SubLight( int light_id, float r, float g, float b);
void HeadLight( int light_id, float r, float g, float b, float att );
void Fog( int fog_mode, float r, float g, float b, float d, float start, float end );
void SpotLight( int light_id, float r, float g, float b, float att_half, 
	float cutoff, int exponent );

void HeadLight( void );
void Lighting( void );

void DrawImGui();

int time;

inline float _blend( float k, float a, float b )
{ return ( k * a + ( 1.0 - k ) * b ); }


// Our state
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void my_display_code()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void glut_display_func()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();

	// Rendering
	ImGui::Render();
	//-----------------------------------------------------

	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}



/*---------------------------------------------------------------- Lighting
 * Lighing
 */
void Lighting( void )
{
	//▼メインライトON: R, G, B
	//MainLight( GL_LIGHT0, 1.0, 1.0, 1.0 );
	MainLight( GL_LIGHT0, 0.8, 0.8, 0.8 );

	//▼サブライトON: R, G, B
	SubLight( GL_LIGHT1, 0.2, 0.2, 0.2 );

	//▼フォグON: R, G, B, 密度, 開始距離, 終了距離
	Fog( GL_EXP, 
		simdata.air_color[0],
		simdata.air_color[1],
		simdata.air_color[2], 
		simdata.air_color[3], 
		simdata.clip_near, simdata.clip_far );
}
/*-------------------------------------------------------------- HeadLight
 * HeadLight
 */
void HeadLight( void )
{
	/*
	glPushMatrix();
	glTranslatef( simdata.body.x, simdata.body.y, simdata.body.z );
	glRotatef( simdata.body.yaw, 0.0, 1.0, 0.0 );
	glRotatef( simdata.body.pitch, 1.0, 0.0, 0.0 );
	glRotatef( simdata.body.roll, 0.0, 0.0, 1.0 );
	*/

	glPushMatrix();
	glTranslatef( simdata.player.pos.x, simdata.player.pos.y, simdata.player.pos.z );
	glRotatef( simdata.player.rot.yaw, 0.0, 1.0, 0.0 );
	glRotatef( simdata.player.rot.pitch, 1.0, 0.0, 0.0 );
	glRotatef( simdata.player.rot.roll, 0.0, 0.0, 1.0 );

	glTranslatef( simdata.head.pos.x, simdata.head.pos.y, simdata.head.pos.z );
	glRotatef( simdata.head.rot.yaw, 0.0, 1.0, 0.0 );
	glRotatef( simdata.head.rot.pitch, 1.0, 0.0, 0.0 );
	glRotatef( simdata.head.rot.roll, 0.0, 0.0, 1.0 );
	//PointLight( GL_LIGHT2, 0.25, 0.25, 0.25, simdata.clip_far );
	//▼ヘッドライトON: R, G, B, 照度半減距離[m]
	SpotLight( GL_LIGHT2, 1.0, 1.0, 1.0, simdata.clip_far, 25.0, 32 );

	glPopMatrix();
}


/*------------------------------------------------------------------- update
 * update - GLUT idle callback function
 *--------*/
void update( void )
{
	time = glutGet( GLUT_ELAPSED_TIME );

    UpdateScene();

	//-------- ready to redraw
	glutPostRedisplay();

	//-------- sync
	//Sleep( 0 ); // NVIDIA Quadro
	Sleep( 16 ); // for cheap video chips @ ~60Hz

	return;
}

/*---------------------------------------------------------------- singleView
 * singleView - draw view on single plane
 */
void singleView( float d )
{
	//-------- projection transformation
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	Projection();

	//-------- viewing transformation
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( d, 0.0, 0.0 );
	Viewing();

	//-------- head light
	//HeadLight();

	//-------- draw
	Lighting();

	DrawScene();

	glPopMatrix();
}

/*------------------------------------------------------------------- display
 * display - GLUT display callback function
 *--------*/
void display( void )
{

	DrawImGui();

	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	//glViewport( 0, 0, window.width, window.height );


	//▼背景色とフォグカラーをブレンド
	glClearColor(
		_blend(simdata.sky_color[3], simdata.sky_color[0], simdata.air_color[0]),
		_blend(simdata.sky_color[3], simdata.sky_color[1], simdata.air_color[1]),
		_blend(simdata.sky_color[3], simdata.sky_color[2], simdata.air_color[2]),
		1.0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	


	//PreDraw();
	//-------- draw --------
	glEnable(GL_DEPTH_TEST); // ---- begin: 
	glEnable(GL_LIGHTING);

	if (cylindrical) {
		cylindricalView(0.0);
	}
	else {
		singleView(0.0);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//Postdraw
	glViewport(0, 0, window.width, window.height);
	
	PostDraw();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	

	glutSwapBuffers();
	//glutPostRedisplay();
	

    return;
}
/*--------------------------------------------------------------------- reshape
 * reshape - GLUT reshape callback function
 *--------*/
void reshape( int width, int height )
{
	ImGui_ImplGLUT_ReshapeFunc(width, height);
	window.width = width;
	window.height = height;
	window.aspect = (float)window.width/window.height;
}
/*---------------------------------------------------------------- initWindow
 * initWindow: initializes OpenGL window
 *--------*/
void initWindow( char *winname )
{
	if( !fullscreen ){// ウィンドウモード
	    window.xo = 100;
	    window.yo = 100;
	    window.width = 1025;//4100/4
	    window.height = 270;//1080/4
	    //-------- window properties
	    glutInitWindowPosition( window.xo, window.yo );
        glutInitWindowSize( window.width, window.height );
	}
	
    //-------- config buffers
	if( stereo ) glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO );
	else         glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    //-------- open window
	window.title = winname;
	glutCreateWindow( window.title );

	if( fullscreen ){// フルスクリーンモード
	    //window.width = 4100;
	    //window.height = 1080;
		//window.xo = 0;
	    //window.yo = 0;
	    glutFullScreen(); //シリンドリカルスクリーンの場合 4100x1080

	}

	if( no_cursor && fullscreen ) glutSetCursor( GLUT_CURSOR_NONE );

	window.width = glutGet( GLUT_WINDOW_WIDTH );
	window.height= glutGet( GLUT_WINDOW_HEIGHT );
	window.xo = glutGet( GLUT_WINDOW_X );
	window.yo = glutGet( GLUT_WINDOW_Y );
	window.aspect = (float)window.width/window.height; // aspect ratio

	//-------- GL mode
	glEnable( GL_NORMALIZE );
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, true );

    return;
}
/*---------------------------------------------------------------------- main
 * main
 *--------*/
int main( int argc, char *argv[] )
{
	
	/// ウィンドウを準備
    glutInit( &argc, argv );

    initWindow( argv[0] );

	//-------- basic callbacks　基本的なコールバック関数の設定

	if( stereo ){
		glutDisplayFunc( Stereo );  // display callback function
	}
	else{
		glutDisplayFunc(display);
	}
	//-------------------------------------------------------------imgui----------------------------------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// imgui init
	ImGui::StyleColorsDark();

	ImGui_ImplGLUT_Init();

	ImGui_ImplOpenGL2_Init();
	//-------------------------------------------------------------imgui----------------------------------------------------------

    glutIdleFunc( update );    // idle callback function
	glutReshapeFunc( reshape ); // reshape callback function

	//-------- keyboard callbacks
    glutKeyboardFunc( charKeyDown ); // character key down callback
	glutKeyboardUpFunc( charKeyUp ); // character key up callback
	glutSpecialFunc( funcKeyDown ); // function key down callback
	glutSpecialUpFunc( funcKeyUp ); // function key up callback
	glutIgnoreKeyRepeat( 1 );   // disable key-repeat

	//-------- mouse callbacks
    glutMouseFunc( mouseClick );       // mouse click callback
	glutPassiveMotionFunc( mouseMotion ); // passive motion callback
	glutMotionFunc( mouseDrag ); // mouse drag callback	

	

    printf( "[H]:Help\n" );     // indicate help instruction

	
   InitScene(); //★状態の初期化
	
	time = glutGet( GLUT_ELAPSED_TIME );

	printf( "//////// プログラムを終了するときには[Q]を押してください////////\n" );
	printf( "\nでは、Enterキーを押すとプログラムがスタートします\n" );
	getchar();
	


    glutMainLoop(); // run main loop

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();


	
    return 0;
}
/******************************************************************************
 * end of source code */
