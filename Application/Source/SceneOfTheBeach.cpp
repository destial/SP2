#include "SceneOfTheBeach.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneOfTheBeach::SceneOfTheBeach() {}

SceneOfTheBeach::~SceneOfTheBeach() {}

void SceneOfTheBeach::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(50, 20, 50);
	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(-50, 20, -50);

	InitGL();

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Sandtexture.tga");

	meshList[GEO_OCEAN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_OCEAN]->textureID = LoadTGA("Image//Ocean.tga");

	meshList[GEO_ROAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_ROAD]->textureID = LoadTGA("Image//RoadTopDown.tga");

	meshList[GEO_DRIZZLE] = MeshBuilder::GenerateOBJMTL("Drizzle", "OBJ//drizzle.obj", "OBJ//drizzle.mtl");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 50.1f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//BeachFront.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 50.1f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//BeachBack.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 50.1f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//BeachBottom.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 50.1f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//BeachTop.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 50.1f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//BeachLeft.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 50.1f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//BeachRight.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.5f, 0.3f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//blueblacktextbox.tga");

	meshList[GEO_UI2] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_UI2]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_CRAB] = MeshBuilder::GenerateOBJMTL("Crab", "OBJ//crab.obj", "OBJ//crab.mtl");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("Tree", "OBJ//palmtree.obj", "OBJ//palmtree.mtl");
	meshList[GEO_TREE]->transform.Translate(10, 0, 0);
	meshList[GEO_TREE]->transform.Scale(0.5, 0.2, 0.5);

	meshList[GEO_TREE2] = MeshBuilder::GenerateOBJMTL("Tree", "OBJ//palmtree.obj", "OBJ//palmtree.mtl");
	meshList[GEO_TREE2]->transform.Translate(-25, 0, 20);;
	meshList[GEO_TREE2]->transform.Scale(0.5, 0.2, 0.5);

	meshList[GEO_TREE3] = MeshBuilder::GenerateOBJMTL("Tree", "OBJ//palmtree.obj", "OBJ//palmtree.mtl");
	meshList[GEO_TREE3]->transform.Translate(20, 0, -30);
	meshList[GEO_TREE3]->transform.Scale(0.5, 0.2, 0.5);

	meshList[GEO_DOLPHIN] = MeshBuilder::GenerateOBJMTL("Dolphin", "OBJ//Dolphin.obj", "OBJ//Dolphin.mtl");

	meshList[GEO_SHIP] = MeshBuilder::GenerateOBJMTL("Ship", "OBJ//ship_light.obj", "OBJ//ship_light.mtl");

	meshList[GEO_UMBRELLA] = MeshBuilder::GenerateOBJMTL("Umbrella", "OBJ//Umbrella.obj", "OBJ//Umbrella.mtl");

	meshList[GEO_CABIN] = MeshBuilder::GenerateOBJ("Cabin", "OBJ//beachcabin.obj");
	meshList[GEO_CABIN]->textureID = LoadTGA("Image//BeachCabin.tga");
	meshList[GEO_CABIN]->transform.Translate(15, -4, 40);
	meshList[GEO_CABIN]->transform.Scale(3, 3, 3);

	//Reset all variables
	Reset();

	Application::log("Scene beach initialized");
}

void SceneOfTheBeach::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneOfTheBeach::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneOfTheBeach::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
		characterSpacing.SetToTranslation(0.5f + i * 0.7f, 0.5f, 0);
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

void SceneOfTheBeach::RenderMeshOnScreen(Mesh* mesh, Color color, float size, float x, float y) {
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

void SceneOfTheBeach::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

void SceneOfTheBeach::Update(double dt, Mouse mouse) {

	if (Application::IsKeyPressed('E'))
	{
		if (camera.position.x <= -35.6 && camera.position.x >= -41.6 && camera.position.z <= 34 && camera.position.z >= 28.8)
		{
			sceneBools[OPEN_TEXT_BOX] = true;
		}
		else
		{
			sceneBools[OPEN_TEXT_BOX] = false;
		}
	}

	if (Application::IsKeyPressed('N') && sceneBools[OPEN_TEXT_BOX] == true)
	{
		sceneBools[OPEN_TEXT_BOX] = false;
	}

	if (Application::IsKeyPressed('Y') && sceneBools[OPEN_TEXT_BOX] == true)
	{
		
		Application::sceneswitch = Application::SCENERYAN;
		Application::previousscene = Application::SCENEBEACH;
	}

	//Crab movement 
	sceneFloats[CRAB_SPEED] = (float)(5 * dt);
	
	if (sceneFloats[CRAB_MOVING] < sceneFloats[RANDOM_MOVE] - sceneFloats[CRAB_SPEED] * 1.5 || sceneFloats[CRAB_MOVING] > sceneFloats[RANDOM_MOVE] + sceneFloats[CRAB_SPEED] * 1.5)
	{
		float direction = Direction(sceneFloats[RANDOM_MOVE] - sceneFloats[CRAB_MOVING]);//Randomly chooses a direction
		sceneFloats[CRAB_MOVING] += sceneFloats[CRAB_SPEED] * direction;
	}
	else
	{
		sceneFloats[RANDOM_MOVE] = Math::RandFloatMinMax(-5, 5);//Randomly chooses a number inbetween -5 to 5
	}
	//Dolphin jumping code (Simple rotation)
	if (sceneFloats[ROTATE_DOLPHIN] == 360)
	{
		sceneFloats[ROTATE_DOLPHIN] = 0;
	}
	sceneFloats[ROTATE_DOLPHIN]++;

	camera.prevPosition = camera.position;
	camera.Update(dt, mouse);

	if (camera.position.x > 45)
	{
		Reset();
		Application::sceneswitch = Application::OVERWORLD;
		Application::previousscene = Application::SCENEBEACH;
	}

	//Collision for trees
	if (isNear(meshList[GEO_TREE], 2.f)) {
		// Get the current view vector and current y position
		Vector3 view = (camera.target - camera.position).Normalized();
		float y = camera.position.y;

		// Set the player back to previous position but current y position (only x & z collision)
		camera.position = camera.prevPosition;
		camera.position.y = y;

		// Set the correct target according to player's position and set the car speed to 0
		camera.target = camera.position + view;
	}
	if (isNear(meshList[GEO_TREE2], 2.f)) {
		// Get the current view vector and current y position
		Vector3 view = (camera.target - camera.position).Normalized();
		float y = camera.position.y;

		// Set the player back to previous position but current y position (only x & z collision)
		camera.position = camera.prevPosition;
		camera.position.y = y;

		// Set the correct target according to player's position and set the car speed to 0
		camera.target = camera.position + view;
	}
	if (isNear(meshList[GEO_TREE3], 2.f)) {
		// Get the current view vector and current y position
		Vector3 view = (camera.target - camera.position).Normalized();
		float y = camera.position.y;

		// Set the player back to previous position but current y position (only x & z collision)
		camera.position = camera.prevPosition;
		camera.position.y = y;

		// Set the correct target according to player's position and set the car speed to 0
		camera.target = camera.position + view;
	}
	if (isNear(meshList[GEO_CABIN], 5.f)) {
		// Get the current view vector and current y position
		Vector3 view = (camera.target - camera.position).Normalized();
		float y = camera.position.y;

		// Set the player back to previous position but current y position (only x & z collision)
		camera.position = camera.prevPosition;
		camera.position.y = y;

		// Set the correct target according to player's position and set the car speed to 0
		camera.target = camera.position + view;
	}

}

bool SceneOfTheBeach::isNear(Mesh* mesh, const float& distance) {

	// Get distance between object and camera
	double d = Math::sqrt(Math::Square(mesh->transform.translate.x - camera.position.x) + Math::Square(mesh->transform.translate.z - camera.position.z));
	return (d - (4 * distance)) <= 0;
}

int SceneOfTheBeach::Direction(float value)
{
	//Decides to move in positive/negative direction
	if (value > 0)
	{
		return 1;
	}
	else if (value < 0)
	{
		return -1;
	} 
}

void SceneOfTheBeach::InitGL()
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
	light[0].color.Set(1, 1,  1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
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

void SceneOfTheBeach::InitGLXray()
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
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
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

void SceneOfTheBeach::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneOfTheBeach::RenderSkybox() {
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -50);
	modelStack.Rotate(90, 1, 0, 0);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 50);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 50, 0);
	modelStack.Rotate(180, 1, 0, 0);
	/*modelStack.Rotate(90, 0, 1, 0);*/
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
}

void SceneOfTheBeach::Render()
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
	view.SetToPerspective(camera.orthographic_size, Application::GetWindowWidth() / Application::GetWindowHeight(), 0.1f, 1000.f);
	projectionStack.LoadMatrix(view);

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Scale(4.5, 4.5, 4.5);
	RenderSkybox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 0);
	modelStack.Scale(50, 40, 50);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, -0.03, 0);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, sceneFloats[CRAB_MOVING]);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_OCEAN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, -0.02, 0);
	modelStack.Scale(10, 1, 120);
	RenderMesh(meshList[GEO_ROAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-40, 0.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, -40);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(23, 0.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate((sceneFloats[CRAB_MOVING] * 2), 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 0.5, 20);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(16, 8, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Translate(sceneFloats[CRAB_MOVING], 0, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_CRAB], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 0);
	modelStack.Scale(0.5, 0.2, 0.5);
	RenderMesh(meshList[GEO_TREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 0, -30);
	modelStack.Scale(0.5, 0.2, 0.5);
	RenderMesh(meshList[GEO_TREE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-25, 0, 20);
	modelStack.Scale(0.5, 0.2, 0.5);
	RenderMesh(meshList[GEO_TREE3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-80, -2, 0);
	modelStack.Rotate(sceneFloats[ROTATE_DOLPHIN], 1, 0, 0);
	modelStack.Translate(0, 0, 30);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_DOLPHIN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-sceneFloats[CRAB_MOVING], -5, -80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(sceneFloats[CRAB_MOVING], 1, 0, 0);
	modelStack.Scale(0.7, 0.7, 0.7);
	RenderMesh(meshList[GEO_SHIP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-30, 0, -40);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_UMBRELLA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, -4, 40);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_CABIN], true);
	modelStack.PopMatrix();

	RenderNPC();
	RenderUI();
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
}

void SceneOfTheBeach::RenderNPC()
{
	modelStack.PushMatrix();
	modelStack.Translate(-39.6, 0, 31.4);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_DRIZZLE], true);
	modelStack.PopMatrix();
}

void SceneOfTheBeach::RenderUI()
{
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (sceneBools[OPEN_TEXT_BOX] == true)
	{
		RenderMeshOnScreen(meshList[GEO_UI], Colors::WHITE, 50, 40, 8 * h / 600); // 40 screenx
		RenderTextOnScreen(meshList[GEO_TEXT], "Would you like to go to Shark Island?", Colors::WHITE, 2, 5, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], "(Y) Yes   (N) No", Colors::WHITE, 2, 10, 2); //X 1.5 AND Z 19.5
		RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	}

	
	RenderMeshOnScreen(meshList[GEO_UI2], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(Player::getHealth()), Colors::BLACK, 2, 0.5, 19 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:" + std::to_string(Player::getAmmo()), Colors::BLACK, 2, 0.5, 18 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:" + std::to_string(Player::getMoney()), Colors::BLACK, 2, 0.5, 17 * h / 600);
}

void SceneOfTheBeach::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneOfTheBeach::Reset() {
	Mtx44 projection;
	projection.SetToPerspective(45.f, Application::GetWindowWidth() / Application::GetWindowHeight(), 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(40, 3, -40), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);
	camera.orthographic_size = 45.f;

	for (unsigned i = 0; i < NUM_SCENE_BOOLS; i++) {
		sceneBools[i] = false;
	}
	for (unsigned i = 0; i < NUM_SCENE_FLOATS; i++) {
		sceneFloats[i] = 0;
	}
}