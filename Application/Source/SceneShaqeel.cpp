#include "SceneShaqeel.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneShaqeel::SceneShaqeel() {}

SceneShaqeel::~SceneShaqeel() {}

void SceneShaqeel::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Generate shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 10, 0);
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(200, 0, 0);

	InitGL();

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color.tga");

	meshList[GEO_QUAD1] = MeshBuilder::GenerateQuad("quad1", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD1]->textureID = LoadTGA("Image//RoadTopDown.tga");

	meshList[GEO_QUAD2] = MeshBuilder::GenerateQuad("quad2", Color(1.6, 0.82, 0.45), 1.f);

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 4, 4, 4);
	meshList[GEO_CUBE]->material.kAmbient.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kShininess = 1.f;

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Colors::BLUE, 30, 30, 0.5);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;

	meshList[GEO_TRUCK] = MeshBuilder::GenerateOBJ("truck", "OBJ//NewTruck2.obj"); // Try 1 first
	meshList[GEO_TRUCK]->textureID = LoadTGA("Image//BlueVehicle.tga");

	meshList[GEO_BUS] = MeshBuilder::GenerateOBJ("bus", "OBJ//NewBus.obj"); // Try 1 first
	meshList[GEO_BUS]->textureID = LoadTGA("Image//Vehicle_Silver.tga");

	meshList[GEO_CAR1] = MeshBuilder::GenerateOBJ("Car1", "OBJ//NewCar1.obj"); // Try 1 first
	meshList[GEO_CAR1]->textureID = LoadTGA("Image//GreenVehicle.tga");

	meshList[GEO_CAR2] = MeshBuilder::GenerateOBJ("Car2", "OBJ//NewCar2.obj"); // Try 1 first
	meshList[GEO_CAR2]->textureID = LoadTGA("Image//RedVehicle.tga");

	meshList[GEO_PLANE] = MeshBuilder::GenerateOBJ("Plane", "OBJ//planewithoutwheels.obj"); // Try 1 first
	meshList[GEO_PLANE]->textureID = LoadTGA("Image//whitemistcolour.tga");

	meshList[GEO_BUSH] = MeshBuilder::GenerateOBJ("Bush", "OBJ//Bush_2.obj"); // Try 1 first
	meshList[GEO_BUSH]->textureID = LoadTGA("Image//Tree_Texture2.tga");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree_4.obj"); // Try 1 first
	meshList[GEO_TREE]->textureID = LoadTGA("Image//Tree_Texture2.tga");

	meshList[GEO_BENCH] = MeshBuilder::GenerateOBJ("Bench", "OBJ//NewBench1.obj"); // Try 1 first
	meshList[GEO_BENCH]->textureID = LoadTGA("Image//bench.tga");

	meshList[GEO_STREETLIGHT] = MeshBuilder::GenerateOBJ("Bench", "OBJ//StreetLight.obj"); // Try 1 first
	meshList[GEO_STREETLIGHT]->textureID = LoadTGA("Image//StreetLight.tga");

	meshList[GEO_MART] = MeshBuilder::GenerateOBJ("Mart", "OBJ//newmartoffset.obj"); // Try 1 first
	meshList[GEO_MART]->textureID = LoadTGA("Image//blueColour.tga");
	meshList[GEO_MART]->transform.Translate(-21.3, -2, -0.1);
	meshList[GEO_MART]->transform.Scale(0.6, 0.6, 0.6);

	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Robowithoutarmsandlegs.obj"); // Try 1 first
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBOLEFTLEG] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roboleftleg.obj"); // Try 1 first
	meshList[GEO_ROBOLEFTLEG]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBORIGHTLEG] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roborightleg.obj"); // Try 1 first
	meshList[GEO_ROBORIGHTLEG]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBOLEFTARM] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roboleftarm.obj"); // Try 1 first
	meshList[GEO_ROBOLEFTARM]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBORIGHTARM] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roborightarm.obj"); // Try 1 first
	meshList[GEO_ROBORIGHTARM]->textureID = LoadTGA("Image//robo_normal.tga");
	//LATER RENAME TO BUILDING
	meshList[GEO_CITY1] = MeshBuilder::GenerateOBJ("Mart", "OBJ//NYBuilding.obj"); // Try 1 first
	meshList[GEO_CITY1]->textureID = LoadTGA("Image//NYBuilding.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//MartDoor1.obj"); // Try 1 first
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//RedColour.tga");

	meshList[GEO_TUNNEL] = MeshBuilder::GenerateOBJ("Tunnel", "OBJ//Tunnel1.obj"); // Try 1 first
	meshList[GEO_TUNNEL]->textureID = LoadTGA("Image//DarkGray.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateSkybox("front", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front-space.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateSkybox("back", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back-space.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateSkybox("left", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//right-space.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateSkybox("right", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//left-space.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateSkybox("top", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top-space.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateSkybox("bottom", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom-space.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	Reset();
	Application::log("Scene Shaqeel initialized");
}

void SceneShaqeel::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	} else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	} else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SceneShaqeel::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneShaqeel::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetUIWidth(), 0, Application::GetUIHeight(), -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneShaqeel::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
	if (!mesh || mesh->textureID <= 0) return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetUIWidth(), 0, Application::GetUIHeight(), -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	mesh->Render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneShaqeel::Update(double dt, Mouse mouse) {

	if (Application::previousscene != Application::SCENESHAQ) {
		InitGL();
	}
	// vehicle movement using translate in render
	sceneFloats[F_TRANSLATE_TRUCK_Z] += (float)(7 * dt); // 3.87 15.7
	sceneFloats[F_TRANSLATE_BUS_Z] -= (float)(7 * dt);
	sceneFloats[F_TRANSLATE_CAR1_Z] += (float)(10 * dt);
	sceneFloats[F_TRANSLATE_CAR2_Z] -= (float)(15 * dt);
	sceneFloats[F_TRANSLATE_SPHERE_Z] += (float)(2.5 * dt);
	sceneFloats[F_TRANSLATE_SPHERE_Z_2] -= (float)(2.75 * dt);
	sceneFloats[F_TRANSLATE_PLANE_X] -= (float)(7 * dt);
	sceneFloats[F_TRANSLATE_PLANE_Z] -= (float)(1.2 * dt);

	// if vehicle travels to a certain point it will respawn and start again
	if (sceneFloats[F_TRANSLATE_TRUCK_Z] >= 40)
	{
		sceneFloats[F_TRANSLATE_TRUCK_Z] = -40;
	}

	if (sceneFloats[F_TRANSLATE_CAR1_Z] >= 40)
	{
		sceneFloats[F_TRANSLATE_CAR1_Z] = -40;
	}

	if (sceneFloats[F_TRANSLATE_BUS_Z] <= -40)
	{
		sceneFloats[F_TRANSLATE_BUS_Z] = 40;
	}

	if (sceneFloats[F_TRANSLATE_CAR2_Z] <= -40)
	{
		sceneFloats[F_TRANSLATE_CAR2_Z] = 40;
	}

	if (sceneFloats[F_TRANSLATE_PLANE_X] <= -40)
	{
		sceneFloats[F_TRANSLATE_PLANE_X] = 40;
		sceneFloats[F_TRANSLATE_PLANE_Z] = 10;
	}

	if (sceneFloats[F_TRANSLATE_SPHERE_Z] >= 25)
	{
		sceneFloats[F_TRANSLATE_SPHERE_Z] = -25;
	}

	if (sceneFloats[F_TRANSLATE_SPHERE_Z_2] <= -30)
	{
		sceneFloats[F_TRANSLATE_SPHERE_Z_2] = 20;
	}

	// press e to open the door
	if (Application::IsKeyPressed('E'))
	{
		// using the location of where the camera is 
		if (camera.position.x <= -10 && camera.position.x >= -15 && camera.position.z <= 1.5 && camera.position.z >= -1.5)
		{
			sceneBools[B_DOOR_OPENED] = true;
		}


	}

	// bool to ensure the door cant open infinitely
	if (sceneBools[B_DOOR_OPENED] == true)
	{
		if (!sceneBools[B_STOP_OPEN_DOOR])
		{
			sceneFloats[F_ROTATE_DOOR] -= (float)(40 * dt);
		}

		if (sceneFloats[F_ROTATE_DOOR] <= -30)
		{
			sceneBools[B_STOP_OPEN_DOOR] = true;
		}
	}

	if (sceneBools[B_STOP_OPEN_DOOR] == true)
	{
		sceneFloats[F_TRANSLATE_WORD_Y] = 2.5;
	}

	// F to enter the mart using same x and z coords as opening door
	if (Application::IsKeyPressedOnce('F') && camera.position.x <= -10 && camera.position.x >= -15 && 
		camera.position.z <= 1.5 && camera.position.z >= -1.5 && sceneBools[B_STOP_OPEN_DOOR] == true) {
		Application::sceneswitch = Application::SCENESHAQLER;
		Application::previousscene = Application::SCENESHAQ;
	}

	if (Application::IsKeyPressedOnce(VK_ESCAPE)) {
		Application::sceneswitch = Application::STARTSCENE;
	}

	// robot movement

	if (sceneBools[B_LEFT_LEG_LIMIT] == true) // this bool statement is used for all of robo limbs
	{
		sceneFloats[F_ROTATE_LEFT_LEG] += 1;
		if (sceneFloats[F_ROTATE_LEFT_LEG] > 30) // ensures limbs dont go too far and will rotate back and forth
		{
			sceneBools[B_LEFT_LEG_LIMIT] = false;
		}
	}
	else if (sceneBools[B_LEFT_LEG_LIMIT] == false)
	{
		sceneFloats[F_ROTATE_LEFT_LEG] -= 1;
		if (sceneFloats[F_ROTATE_LEFT_LEG] < -30)
		{
			sceneBools[B_LEFT_LEG_LIMIT] = true;
		}
	}

	camera.prevPosition = camera.position;
	camera.Update(dt, mouse);

	// collision of the mart
	if (isNear(meshList[GEO_MART], 2.f)) {
		// Get the current view vector and current y position
		Vector3 view = (camera.target - camera.position).Normalized();
		float y = camera.position.y;

		// Set the player back to previous position but current y position (only x & z collision)
		camera.position = camera.prevPosition;
		camera.position.y = y;

		// Set the correct target according to player's position and set the car speed to 0
		camera.target = camera.position + view;
	}

	if (camera.position.x <= 8 &&
		camera.position.x >= -8 &&
		camera.position.z <= 30 &&
		camera.position.z >= 24)
	{
		Reset();
		Application::sceneswitch = Application::OVERWORLD;
		Application::previousscene = Application::SCENESHAQ;
	}
}

bool SceneShaqeel::isNear(Mesh* mesh, const float& distance) {

	// Get distance between object and camera
	double d = Math::sqrt(Math::Square(mesh->transform.translate.x - camera.position.x) + Math::Square(mesh->transform.translate.z - camera.position.z));
	return (d - (4 * distance)) <= 0;
}

void SceneShaqeel::InitGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");



	// Make sure you pass uniform parameters after glUseProgram()

	//Replace previous code
	//light[0].type = Light::LIGHT_POINT;
	//light[0].position.Set(0, 0, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 2;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	//light[1].type = Light::LIGHT_POINT;
	//light[1].position.Set(0, 0, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);
	glUseProgram(m_programID);
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
}

void SceneShaqeel::InitGLXray()
{


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//TextAlpha.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");



	// Make sure you pass uniform parameters after glUseProgram()

	//Replace previous code
	//light[0].type = Light::LIGHT_POINT;
	//light[0].position.Set(0, 0, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	//light[1].type = Light::LIGHT_POINT;
	//light[1].position.Set(0, 0, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);
	glUseProgram(m_programID);
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
}

void SceneShaqeel::Update(double dt)
{
	Mouse mouse;

	Update(dt, mouse);

	/*sceneFloats[F_TRANSLATE_TRUCK_Z] -= (float)(30 * dt);*/

}

void SceneShaqeel::RenderSkybox() {
	float translate = 50;
	float scaleVal = (translate * 2) + (translate * 0.01f);
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y + translate, camera.position.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(270, 0, 0, 1);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y - translate, camera.position.z);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y - 0.4, camera.position.z - translate);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y - 0.4, camera.position.z + translate);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x + translate, camera.position.y - 0.4, camera.position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x - translate, camera.position.y - 0.4, camera.position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
}

void SceneShaqeel::Render()
{
	//Clear the color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (light[0].type == Light::LIGHT_DIRECTIONAL) {
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;

		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	} else if (light[0].type == Light::LIGHT_SPOT) {
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	} else {
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL) {
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	} else if (light[1].type == Light::LIGHT_SPOT) {
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);

		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	} else {
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	Mtx44 view;
	view.SetToPerspective(camera.orthographic_size, 800.f / 600.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(view);
	RenderSkybox();

	// items are rendered in the functions below
	RenderQuad();
	Rendervehicles();
	Rendertrees();
	Rendercityobjects();
	RenderMytext();
	RenderNPC();
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	RenderUI();
}

void SceneShaqeel::RenderUI() {
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	RenderMeshOnScreen(meshList[GEO_UI], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(Player::getHealth()), Colors::BLACK, 2, 0.5 * w / 800, 19 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:" + std::to_string(Player::getAmmo()), Colors::BLACK, 2, 0.5 * w / 800, 18 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:" + std::to_string(Player::getMoney()), Colors::BLACK, 2, 0.5 * w / 800, 17 * h / 600);
}

void SceneShaqeel::RenderQuad()
{
	// road quad is smaller and very slightly above the ground quad
	modelStack.PushMatrix();
	modelStack.Translate(0, -2, 0);
	modelStack.Scale(10, 10, 40);
	RenderMesh(meshList[GEO_QUAD1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -2.02, 0);
	modelStack.Scale(30, 30, 40);
	RenderMesh(meshList[GEO_QUAD2], true);
	modelStack.PopMatrix();
}

// vehicles such as truck and plane are rendered here
void SceneShaqeel::Rendervehicles()
{
	modelStack.PushMatrix();
	modelStack.Translate(-1.3, -0.8, sceneFloats[F_TRANSLATE_TRUCK_Z]);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.022, 0.022, 0.022);
	RenderMesh(meshList[GEO_TRUCK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5, -0.6, sceneFloats[F_TRANSLATE_BUS_Z]);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.038, 0.038, 0.038);
	RenderMesh(meshList[GEO_BUS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-6, -0.5, sceneFloats[F_TRANSLATE_CAR1_Z]);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.095, 0.095, 0.095);
	RenderMesh(meshList[GEO_CAR1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.5, -0.7, sceneFloats[F_TRANSLATE_CAR2_Z]);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.09, 0.09, 0.09);
	RenderMesh(meshList[GEO_CAR2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(sceneFloats[F_TRANSLATE_PLANE_X], 42, sceneFloats[F_TRANSLATE_PLANE_Z]);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_PLANE], true);
	modelStack.PopMatrix();
}

// trees and bushes
void SceneShaqeel::Rendertrees()
{
	modelStack.PushMatrix();
	modelStack.Translate(-20, -2.03, 20);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, -2.03, -20);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-27.5, -2.03, 28);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-27.5, -2.03, -28);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, -2.03, 20);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, -2.03, -20);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(27.5, -2.03, 28);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(27.5, -2.03, -28);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.25, 0.25);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();
}

// buildings, streetlight, mart etc...
void SceneShaqeel::Rendercityobjects()
{
	modelStack.PushMatrix();
	modelStack.Translate(-18, -2, 23);
	//modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BENCH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-18, -2, -23);
	//modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BENCH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(18, -2, 23);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BENCH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(18, -2, -23);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BENCH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_MART]->transform.translate.x, meshList[GEO_MART]->transform.translate.y, meshList[GEO_MART]->transform.translate.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(meshList[GEO_MART]->transform.scale.x, meshList[GEO_MART]->transform.scale.y, meshList[GEO_MART]->transform.scale.z);
	RenderMesh(meshList[GEO_MART], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-15, -2, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_DOOR], 0, 1, 0);
	modelStack.Scale(0.61, 0.61, 0.61);
	RenderMesh(meshList[GEO_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.8, -2, -50); // X 3.6
	/*modelStack.Rotate(180, 0, 1, 0);*/
	modelStack.Scale(0.77, 0.77, 0.77);
	RenderMesh(meshList[GEO_TUNNEL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.8, -2, 50); // X 3.6
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.77, 0.77, 0.77);
	RenderMesh(meshList[GEO_TUNNEL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(22.1, -2, 0);
	modelStack.Scale(0.4, 0.4, 0.4);
	RenderMesh(meshList[GEO_CITY1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(22.1, -2, -9);
	modelStack.Scale(0.38, 0.38, 0.38);
	RenderMesh(meshList[GEO_CITY1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(22.1, -2, 9);
	modelStack.Scale(0.38, 0.38, 0.38);
	RenderMesh(meshList[GEO_CITY1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 0, -0.18); // X 3.6
	/*modelStack.Rotate(180, 0, 1, 0);*/
	modelStack.Scale(0.5, 1, 0.9);
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-11.7, -2, -13.2);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_STREETLIGHT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(11.7, -2, 13.2);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_STREETLIGHT], true);
	modelStack.PopMatrix();

}

// text for door
void SceneShaqeel::RenderMytext()
{
	modelStack.PushMatrix();
	modelStack.Translate(-14.8, sceneFloats[F_TRANSLATE_WORD_Y], 0.9); // text will appear once door is open
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.15, 0.15, 0.15);
	RenderText(meshList[GEO_TEXT], " Press F to Enter", Colors::WHITE);
	modelStack.PopMatrix();
}

// robos
void SceneShaqeel::RenderNPC()
{
	// robo done with hierarch modelling with small sphere at the centre
	modelStack.PushMatrix();
	modelStack.Translate(-13.2, 0, sceneFloats[F_TRANSLATE_SPHERE_Z]);
	modelStack.Scale(0.25, 0.25, 0.25);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// 2nd robot

	modelStack.PushMatrix();
	modelStack.Translate(13.2, 0, sceneFloats[F_TRANSLATE_SPHERE_Z_2]);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.25, 0.25, 0.25);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_LEFT_LEG], 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneShaqeel::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneShaqeel::Reset() {
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(5, 0.4, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)30);
	camera.orthographic_size = 45.f;

	// initialised
	sceneFloats[F_TRANSLATE_TRUCK_Z] = 10;
	sceneFloats[F_BUS_Z] = 0;
	sceneFloats[F_TRANSLATE_CAR1_Z] = -10;
	sceneFloats[F_TRANSLATE_CAR2_Z] = 20;
	sceneFloats[F_ROTATE_DOOR] = 90;
	sceneFloats[F_TRANSLATE_WORD_Y] = 1000;
	sceneFloats[F_ROTATE_LEFT_LEG] = 0;
	sceneFloats[F_ROTATE_RIGHT_LEG] = 0;
	sceneFloats[F_TRANSLATE_SPHERE_Z] = -19.6;
	sceneFloats[F_TRANSLATE_SPHERE_Z_2] = 19.6;
	sceneFloats[F_TRANSLATE_PLANE_X] = 40;
	sceneFloats[F_TRANSLATE_PLANE_Z] = 10;

	sceneBools[B_LEFT_LEG_LIMIT] = false;
	sceneBools[B_LEFT_LEG_LIMIT_2] = false;
	sceneBools[B_RIGHT_LEG_LIMIT] = false;
}