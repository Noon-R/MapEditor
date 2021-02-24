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


void drawMap(int cellState,ObjDataT* obj) {
	
	color_t currentColor ;
	currentColor = simdata.paintCols[ min( cellState,  (int)(simdata.paintCols.size()-1))];
	setObjColor(obj,
		currentColor.red,
		currentColor.green,
		currentColor.blue);

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

	bool isWindowFocused = false;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	//ImGui::ShowDemoWindow();

	{
		ImGui::Begin("Controller");
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
		isWindowFocused |= ImGui::IsWindowFocused();
		
		//Mapの縦横の決定
		static int width = ezMap_getMapData()->field_width;
		static int height = ezMap_getMapData()->field_height;

		ImGui::DragInt("Map Width", &width, 1, 1, 256, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragInt("Map Height", &height, 1, 1, 256, "%d", ImGuiSliderFlags_AlwaysClamp);
		
		if (ImGui::Button("UpdateMapSize")) {
			ezMap_dataResize(width,height, true);
		}
		
		ImGui::InputTextWithHint("input fileName", "enter text here", simdata.fileName, IM_ARRAYSIZE(simdata.fileName));
		

		if (ImGui::Button("ExportMapData")) {
			bool doneSave = ezMap_save(simdata.fileName);

			if(doneSave)ImGui::OpenPopup("Complete Export");

		}

		if (ImGui::BeginPopupModal("Complete Export", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			isWindowFocused |= ImGui::IsWindowFocused();
			if (strcmp(simdata.fileName,"")==0) {
				ImGui::Text("\n Complete Export Mapdata!!\n \" mapData.txt  \" is in mapData\n\n");
			}
			else {
				ImGui::Text("\n Complete Export Mapdata!!\n \" %s \" is in mapData\n\n", simdata.fileName);
			}
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}


		if (ImGui::Button("LoadMapData")) {
			bool doneLoad = ezMap_load(simdata.fileName);

			if (!doneLoad) ImGui::OpenPopup("Failed Load File");

			width = ezMap_getMapData()->field_width;
			height = ezMap_getMapData()->field_height;
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Failed Load File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			isWindowFocused |= ImGui::IsWindowFocused();
			ImGui::Text("Can't Load \" %s \" File. Please Confirm FileName and FilePath \n\n", simdata.fileName);
			ImGui::Text("Make New Data : width->32, height->32\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		
			ImGui::EndPopup();
		}
		//-------------color & state ----------------
		

		ImGui::Text("\n");
		ImGui::Separator();
		ImGui::Text("\n");


		color_t cuurentCol = simdata.paintCols[simdata.currentPaintNum];
		float displayCol[4] = { cuurentCol.red, cuurentCol.green, cuurentCol.blue, cuurentCol.alpha };
		ImGui::ColorEdit4("CurrentColor", displayCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker);

		if (ImGui::BeginTable("table_item_width", 3, ImGuiTableFlags_Borders  | ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("No.");
			ImGui::TableSetupColumn("Color");
			ImGui::TableSetupColumn("selectButton");
			ImGui::TableHeadersRow();

			for (int row = 0; row < (int)simdata.paintCols.size(); row++)
			{
				ImGui::TableNextRow();
				if (row == 0)
				{
					// Setup ItemWidth once (instead of setting up every time, which is also possible but less efficient)
					ImGui::TableSetColumnIndex(0);
					ImGui::PushItemWidth(ImGui::CalcTextSize("A").x * 3.0f); // Small
					ImGui::TableSetColumnIndex(1);
					ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
					ImGui::TableSetColumnIndex(2);
					ImGui::PushItemWidth(-FLT_MIN); // Right-aligned
				}

				// Draw our contents
				static float dummy_f = 0.0f;
				ImGui::PushID(row);

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", row, 0.0f, 1.0f);

				ImGui::TableSetColumnIndex(1);
				color_t col = simdata.paintCols[row];
				float rowCol[4] = { col.red, col.green, col.blue, col.alpha };
				ImGui::ColorEdit4("color", rowCol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				simdata.paintCols[row] = { rowCol[0], rowCol[1], rowCol[2], rowCol[3] };

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button("Select")) {
					simdata.currentPaintNum = row;
				}
				ImGui::PopID();
			}
			ImGui::EndTable();


			static float col[4] = { 0.2, 0.35, 0.2,1.0 };
			ImGui::ColorEdit4("AddColor", col);

			if (ImGui::Button("AddRow")) {
				color_t addColor = color_t(col[0],col[1],col[2],col[3] );
				simdata.paintCols.push_back(addColor);

			}

		}
		ImGui::End();
	}

	{
		ImGui::Begin("CameraController");
		ImGui::SetNextWindowSize(ImVec2(900, 680), ImGuiCond_FirstUseEver);

		isWindowFocused |= ImGui::IsWindowFocused();

		ImGui::DragFloat("Map Viewing Angle", &simdata.viewing, 0.1f);
		ImGui::End();
	}
	ImGui::Render();

	simdata.isImGuiWIndowFocused = isWindowFocused;
}
