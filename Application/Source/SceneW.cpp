#include "SceneW.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneW::SceneW() {}

SceneW::~SceneW() {}

void SceneW::Init() {
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Generate shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 0, 0);
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 0, 0);

	InitGL();

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(-46,3.5,-48), Vector3(1, 0.5, 1), Vector3(0, 1, 0), 50.f);


	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", WHITE, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//tron_ft.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", WHITE, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//tron_bk.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", WHITE, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//tron_rt.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", WHITE, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//tron_lf.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", WHITE, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//tron_up.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", WHITE, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//tron_dn.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", WHITE, 1.f, 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateOBJMTL("WallType1", "OBJ//wall.obj", "OBJ//wall.mtl");
	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJMTL("Door", "OBJ//doorway.obj", "OBJ//doorway.mtl");
	meshList[GEO_WALLDOOR] = MeshBuilder::GenerateOBJMTL("WallDoor", "OBJ//wallDoorway.obj", "OBJ//wallDoorway.mtl");
	meshList[BOX] = MeshBuilder::GenerateOBJMTL("Box", "OBJ//cardboardBoxClosed.obj", "OBJ//cardboardBoxClosed.mtl");
	meshList[MWALL] = MeshBuilder::GenerateCube("MazeWall",YELLOW, 1, 1, 1);
	meshList[MWALL]->material.kAmbient.Set(.03f, .03f, .03f);
	meshList[MWALL]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[MWALL]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[MWALL]->material.kShininess = .3f;
	meshList[CHESTTOP] = MeshBuilder::GenerateOBJ("Chest Top", "OBJ//chestTopPart.obj"); // Try 1 first
	meshList[CHESTTOP]->textureID = LoadTGA("Image//ChestTexture.tga");

	meshList[CHESTBOTTOM] = MeshBuilder::GenerateOBJ("Chest Top", "OBJ//chestBottomPart.obj"); // Try 1 first
	meshList[CHESTBOTTOM]->textureID = LoadTGA("Image//ChestTexture.tga");

	meshList[CAMERA] = new Mesh("camera");
	meshList[CAMERA]->type = Mesh::CAMERA;

	rotateChest = 0;
	rotateChest2 = 0;
	rotateChest3 = 0;
	rotateChest4 = 0;
	rotateChest5 = 0;
	countChest = 0;

	Chestlimit = false;
	Chestlimit2 = false;
	Chestlimit3 = false;
	Chestlimit4 = false;
	Chestlimit5 = false;

	sceneManager = new SceneManager(this, camera.bounds);
	CreateMaze();
	GameObject* cameraObject = new GameObject(meshList[CAMERA]);
	cameraObject->transform->translate = camera.position;
	cameraObject->camera = 1;
	sceneManager->push(cameraObject);
	cameraObject->id = sceneManager->totalObjects;

	Application::log("Scene Walton initialized");
}

void SceneW::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneW::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneW::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneW::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

void SceneW::Update(double dt, Mouse mouse) {
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);

	else if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode

	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	static const float LSPEED = 40.f;
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('E'))
	{
		if (Chestlimit == false)
		{
			rotateChest -= (float)(40 * dt);
			if (rotateChest <= -70)
			{
				Chestlimit = true;
				Chestlimit2 = false;
				Chestlimit3 = true;
				Chestlimit4 = true;
				Chestlimit5 = true;
				countChest++;
			}
		}

		else if (Chestlimit2 == false)
		{
			rotateChest2 -= (float)(40 * dt);
			if (rotateChest2 <= -70)
			{
				Chestlimit = true;
				Chestlimit2 = true;
				Chestlimit3 = false;
				Chestlimit4 = true;
				Chestlimit5 = true;
				countChest++;
			}
		}

		else if (Chestlimit3 == false)
		{
			rotateChest3 -= (float)(40 * dt);
			if (rotateChest3 <= -70)
			{
				Chestlimit = true;
				Chestlimit2 = true;
				Chestlimit3 = true;
				Chestlimit4 = false;
				Chestlimit5 = true;
				countChest++;
			}

		}

		else if (Chestlimit4 == false)
		{
			rotateChest4 -= (float)(40 * dt);
			if (rotateChest4 <= -70)
			{
				Chestlimit = true;
				Chestlimit2 = true;
				Chestlimit3 = true;
				Chestlimit4 = true;
				Chestlimit5 = false;
				countChest++;
			}

		}

		else if (Chestlimit5 == false)
		{
			rotateChest5 -= (float)(40 * dt);
			if (rotateChest5 <= -70)
			{
				Chestlimit = true;
				Chestlimit2 = true;
				Chestlimit3 = true;
				Chestlimit4 = true;
				Chestlimit5 = true;
				countChest++;
			}

		}
	}

	oldCameraPos = camera.position;
	oldCameraTarget = camera.target;
	camera.Update(dt, mouse);
	
	//sceneManager->split(sceneManager->root);

	DetectCollision();

	sceneManager->deleteAllQuad(sceneManager->root);
	sceneManager->root = new Quad(camera.bounds);
	for (auto o : sceneManager->allObjects) {
		sceneManager->root->push(o);
	}
}

void SceneW::InitGL()
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

void SceneW::DetectCollision() {
	Quad* quad = sceneManager->getQuad(sceneManager->totalObjects);
	if (quad) {
		for (auto object : quad->gameObjects) {
			if (!object->camera) {
				if (isNear(object)) {
					moveBack(object);
				}
			}
		}
	}
}

bool SceneW::isNear(GameObject* object) {
	// Get distance between object and camera
	float d = Math::Square(object->transform->translate.x - camera.position.x) + Math::Square(object->transform->translate.z - camera.position.z);
	return (d - 2.5*object->transform->scale.x <= 0);
}

void SceneW::moveBack(GameObject* object) {
	Vector3 view = (camera.target - camera.position).Normalized();

	float zDistance = Math::sqrt(Math::Square(object->transform->translate.z - camera.position.z));
	float xDistance = Math::sqrt(Math::Square(object->transform->translate.x - camera.position.x));
	if (zDistance < 2.5 * object->transform->scale.x) {
		camera.position.z = camera.prevPosition.z;
	}
	if (xDistance < 2.5 * object->transform->scale.x) {
		camera.position.x = camera.prevPosition.x;
	}

	camera.target = camera.position + view;
}

void SceneW::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneW::RenderSkybox() {
	float translate = 50;
	float scaleVal = 100;//(translate * 2) + (translate * 0.01f);
	modelStack.PushMatrix();
	modelStack.Translate(0, 100, 0);//camera.position.x, camera.position.y + translate, camera.position.z);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);//camera.position.x, camera.position.y - translate, camera.position.z);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -100);//camera.position.x, camera.position.y - 0.4, camera.position.z - translate);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 100);//camera.position.x, camera.position.y - 0.4, camera.position.z + translate);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, 0, 0);//camera.position.x + translate, camera.position.y - 0.4, camera.position.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100,0,0);//camera.position.x - translate, camera.position.y - 0.4, camera.position.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-270, 0, 0, 1);
	modelStack.Scale(scaleVal, scaleVal, scaleVal);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
}

void SceneW::Render()
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

	RenderSkybox();
	RenderRoom();
	RenderBoxes();
	RenderMaze();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, .1, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-49.5, 10, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderText(meshList[GEO_TEXT], "Find the Exit", RED);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-49.5, 9, -2.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderText(meshList[GEO_TEXT], "and", RED);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-49.5, 8, 0.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderText(meshList[GEO_TEXT], "Collect all the", RED);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-49.5, 7, -0.75);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderText(meshList[GEO_TEXT], "treasures", RED);
	modelStack.PopMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], ".", WHITE, 0, 0, -3);
	RenderUI();
}

void SceneW::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	delete sceneManager;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneW::RenderUI() {

	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	RenderMeshOnScreen(meshList[GEO_UI], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:100", BLACK, 2, 0.5, 19 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:100", BLACK, 2, 0.5, 18 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:$100", BLACK, 2, 0.5, 17.3 * h / 600);
}

void SceneW::RenderRoom() {
	// left side of the room
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, -40);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALLDOOR], true);
	modelStack.PopMatrix();

	// Door
	modelStack.PushMatrix();
	modelStack.Translate(-51, 0, -47.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, 50);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(90, 10, 10);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();
	// right side of the room
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, -50);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(100, 10, 10);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();
	// front of the room
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0, -50);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(100, 10, 10);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	// back of the room
	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 50);
	modelStack.Scale(100, 10, 10);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	// Exit Door
	modelStack.PushMatrix();
	modelStack.Translate(26.5, 0, 48);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_DOOR], true);
	modelStack.PopMatrix();

}

void SceneW::RenderBoxes() {
	
	modelStack.PushMatrix();
	modelStack.Translate(-46.5, 2, 45);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(rotateChest, 1, 0, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[CHESTTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-46.5, 0, 45);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[CHESTBOTTOM], true);
	modelStack.PopMatrix();

	// Box 2
	/*modelStack.PushMatrix();
	modelStack.Translate(-20, 0, 16);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[BOX], true);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(-20, 1.4, 15);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Rotate(rotateChest2, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, 15);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTBOTTOM], true);
	modelStack.PopMatrix();

	// Box 3
	//modelStack.PushMatrix();
	//modelStack.Translate(-24, 0, 34);
	////modelStack.Rotate(90, 0, 1, 0);
	//modelStack.Scale(10, 10, 10);
	//RenderMesh(meshList[BOX], true);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-24, 1.4, 35);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(rotateChest3, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-24, 0, 35);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTBOTTOM], true);
	modelStack.PopMatrix();

	// Box 4
	/*modelStack.PushMatrix();
	modelStack.Translate(33.5, 0, -35);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[BOX], true);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(33.5, 1.4, -35);
	modelStack.Rotate(rotateChest4, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(33.5, 0, -35);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTBOTTOM], true);
	modelStack.PopMatrix();

	// Box 5
	/*modelStack.PushMatrix();
	modelStack.Translate(20, 0, -5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[BOX], true);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(20, 1.4, -5);
	modelStack.Rotate(rotateChest5, 1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 0, -5);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[CHESTBOTTOM], true);
	modelStack.PopMatrix();
}

void SceneW::RenderMaze() {

	// maze
	for (auto object : sceneManager->root->gameObjects)
	{
		if (object) {
			if (object->mesh == meshList[MWALL]) {
				modelStack.PushMatrix();
				modelStack.Translate(object->transform->translate.x, object->transform->translate.y, object->transform->translate.z);
				modelStack.Scale(object->transform->scale.x, object->transform->scale.y, object->transform->scale.z);
				RenderMesh(object->mesh, true);
				modelStack.PopMatrix();
			}
		}
	}
}

void SceneW::CreateMaze() {
	GameObject* object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -50);
	object->transform->Scale(5, 5, 5);
	object->id = 1;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -45);;
	object->transform->Scale(5, 5, 5);
	object->id = 2;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 3;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -35);
	object->transform->Scale(5, 5, 5);
	object->id = 4;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 5;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -25);
	object->transform->Scale(5, 5, 5);
	object->id = 6;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 7;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 8;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 9;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 10;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 11;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 12;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 13;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 14;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 25);
	object->transform->Scale(5, 5, 5);
	object->id = 15;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 16;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 17;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 18;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 45);
	object->transform->Scale(5, 5, 5);
	object->id = 19;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-40, 2.5, 50);
	object->transform->Scale(5, 5, 5);
	object->id = 20;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-35, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 21;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 22;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-25, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 23;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 24;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 25;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 26;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 27;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 28;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-25, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 29;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 30;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 31;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 32;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 33;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 34;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 35;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 36;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -25);
	object->transform->Scale(5, 5, 5);
	object->id = 37;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 38;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -35);
	object->transform->Scale(5, 5, 5);
	object->id = 39;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 40;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-25, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 41;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 42;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 43;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 44;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 45;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(0, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 46;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -50);
	object->transform->Scale(5, 5, 5);
	object->id = 47;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -45);
	object->transform->Scale(5, 5, 5);
	object->id = 48;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 49;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -35);
	object->transform->Scale(5, 5, 5);
	object->id = 50;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 51;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(10, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 52;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 53;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(0, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 54;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 55;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 56;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 57;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 58;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -25);
	object->transform->Scale(5, 5, 5);
	object->id = 59;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 60;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 61;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 62;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 63;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 64;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 65;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 66;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 67;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 68;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 69;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 70;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 71;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 25);
	object->transform->Scale(5, 5, 5);
	object->id = 72;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 73;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 74;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-15, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 75;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 76;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-25, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 77;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 78;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 79;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-30, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 80;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-25, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 81;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-20, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 82;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 83;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 84;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 85;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(0, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 86;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 87;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(10, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 88;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 89;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 45);
	object->transform->Scale(5, 5, 5);
	object->id = 90;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 50);
	object->transform->Scale(5, 5, 5);
	object->id = 91;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 92;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 93;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 94;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(10, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 95;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 96;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 25);
	object->transform->Scale(5, 5, 5);
	object->id = 97;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 98;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 99;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 100;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 101;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 102;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 103;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 104;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(0, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 105;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 106;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 107;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-10, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 108;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(-5, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 109;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(0, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 110;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(5, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 111;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(10, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 112;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 113;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 114;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 115;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -25);
	object->transform->Scale(5, 5, 5);
	object->id = 116;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 117;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -35);
	object->transform->Scale(5, 5, 5);
	object->id = 118;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 119;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 120;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(35, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 121;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -40);
	object->transform->Scale(5, 5, 5);
	object->id = 122;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -35);
	object->transform->Scale(5, 5, 5);
	object->id = 123;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -30);
	object->transform->Scale(5, 5, 5);
	object->id = 124;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -25);
	object->transform->Scale(5, 5, 5);
	object->id = 125;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -20);
	object->transform->Scale(5, 5, 5);
	object->id = 126;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -15);
	object->transform->Scale(5, 5, 5);
	object->id = 127;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 128;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 129;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 130;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 131;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 132;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 133;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 134;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 25);
	object->transform->Scale(5, 5, 5);
	object->id = 135;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 136;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 137;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(40, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 138;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 45);
	object->transform->Scale(5, 5, 5);
	object->id = 139;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 50);
	object->transform->Scale(5, 5, 5);
	object->id = 140;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 40);
	object->transform->Scale(5, 5, 5);
	object->id = 141;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 35);
	object->transform->Scale(5, 5, 5);
	object->id = 142;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 30);
	object->transform->Scale(5, 5, 5);
	object->id = 143;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 25);
	object->transform->Scale(5, 5, 5);
	object->id = 144;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 145;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 146;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 147;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 148;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 149;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 150;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(30, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 151;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 152;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 153;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -10);
	object->transform->Scale(5, 5, 5);
	object->id = 154;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, -5);
	object->transform->Scale(5, 5, 5);
	object->id = 155;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 0);
	object->transform->Scale(5, 5, 5);
	object->id = 156;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 157;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(20, 2.5, 5);
	object->transform->Scale(5, 5, 5);
	object->id = 158;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(25, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 159;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 10);
	object->transform->Scale(5, 5, 5);
	object->id = 160;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 15);
	object->transform->Scale(5, 5, 5);
	object->id = 161;
	sceneManager->push(object);

	object = new GameObject(meshList[MWALL]);
	object->transform->Translate(15, 2.5, 20);
	object->transform->Scale(5, 5, 5);
	object->id = 162;
	sceneManager->push(object);
}