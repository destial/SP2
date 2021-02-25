#include "OverworldScene.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

OverworldScene::OverworldScene() {}

OverworldScene::~OverworldScene() {}

void OverworldScene::Init() {
	// Clear background color to blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Generate shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	InitGL();

	// Init scene manager
	// Generate necessary meshes and starting transformations
	meshList[GEO_FRONT] = MeshBuilder::GenerateSkybox("front", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front-space.tga");
	meshList[GEO_FRONT]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_BACK] = MeshBuilder::GenerateSkybox("back", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back-space.tga");
	meshList[GEO_BACK]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_LEFT] = MeshBuilder::GenerateSkybox("left", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//right-space.tga");
	meshList[GEO_LEFT]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_RIGHT] = MeshBuilder::GenerateSkybox("right", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//left-space.tga");
	meshList[GEO_RIGHT]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_TOP] = MeshBuilder::GenerateSkybox("top", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top-space.tga");
	meshList[GEO_TOP]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateSkybox("bottom", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom-space.tga");
	meshList[GEO_BOTTOM]->type = Mesh::TYPE::IMAGE;

	meshList[MOON] = MeshBuilder::GenerateSphere("moon", Colors::YELLOW, 30, 30, 1.f);
	meshList[MOON]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->type = Mesh::TYPE::IMAGE;

	meshList[SIDEBAR] = MeshBuilder::GenerateFaceQuad("sidebar", Colors::GRAY, 1.f, 1.f);
	meshList[SIDEBAR]->textureID = LoadTGA("Image//button.tga");
	meshList[SIDEBAR]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//button.tga");
	meshList[GEO_UI]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_GROUND] = MeshBuilder::GenerateGround("ground", Colors::GRAY, 1000.f, 18);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//roadcross.tga");
	meshList[GEO_GROUND]->type = Mesh::TYPE::IMAGE;

	meshList[TRUCK1] = MeshBuilder::GenerateOBJ("truck1", "OBJ//tartruck.obj");
	meshList[TRUCK1]->textureID = LoadTGA("Image//Vehicle_Silver.tga");
	meshList[TRUCK1]->type = Mesh::TYPE::OBJECT;

	meshList[TRUCK2] = MeshBuilder::GenerateOBJ("truck2", "OBJ//carrytruck.obj");
	meshList[TRUCK2]->textureID = LoadTGA("Image//Vehicle_Silver.tga");
	meshList[TRUCK2]->type = Mesh::TYPE::OBJECT;

	meshList[CAR1] = MeshBuilder::GenerateOBJ("car1", "OBJ//NewCar1.obj");
	meshList[CAR1]->textureID = LoadTGA("Image//GreenVehicle.tga");
	meshList[CAR1]->type = Mesh::TYPE::OBJECT;

	meshList[BUS1] = MeshBuilder::GenerateOBJ("bus", "OBJ//bus1.obj");
	meshList[BUS1]->textureID = LoadTGA("Image//Vehicle_Silver.tga");
	meshList[BUS1]->type = Mesh::TYPE::OBJECT;

	meshList[CAR2] = MeshBuilder::GenerateOBJ("car2", "OBJ//NewCar2.obj");
	meshList[CAR2]->textureID = LoadTGA("Image//RedVehicle.tga");
	meshList[CAR2]->type = Mesh::TYPE::OBJECT;

	meshList[SKYSCRAPER2] = MeshBuilder::GenerateOBJ("skyscraper", "OBJ//skyscraper4.obj");
	meshList[SKYSCRAPER2]->textureID = LoadTGA("Image//NewSkyscraper.tga");
	meshList[SKYSCRAPER2]->type = Mesh::TYPE::OBJECT;

	meshList[NY_BUILDING] = MeshBuilder::GenerateOBJ("Mart", "OBJ//NYBuilding.obj");
	meshList[NY_BUILDING]->textureID = LoadTGA("Image//NYBuilding.tga");

	meshList[STREETLIGHT] = MeshBuilder::GenerateOBJ("streetlight", "OBJ//StreetLight.obj");
	meshList[STREETLIGHT]->textureID = LoadTGA("Image//StreetLight.tga");
	meshList[STREETLIGHT]->type = Mesh::TYPE::OBJECT;

	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Robowithoutarmsandlegs.obj");
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBOLEFTLEG] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roboleftleg.obj");
	meshList[GEO_ROBOLEFTLEG]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBORIGHTLEG] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roborightleg.obj");
	meshList[GEO_ROBORIGHTLEG]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBOLEFTARM] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roboleftarm.obj");
	meshList[GEO_ROBOLEFTARM]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[GEO_ROBORIGHTARM] = MeshBuilder::GenerateOBJ("Mart", "OBJ//Roborightarm.obj");
	meshList[GEO_ROBORIGHTARM]->textureID = LoadTGA("Image//robo_normal.tga");

	meshList[PLANE] = MeshBuilder::GenerateOBJ("plane", "OBJ//planewithoutwheels.obj");

	meshList[CAMERA] = new Mesh("camera");
	meshList[CAMERA]->type = Mesh::TYPE::CAMERA;

	Reset();

	Application::log("Overworld Scene initialized");
}

void OverworldScene::RenderMesh(Mesh* mesh, bool enableLight) {
	if (!mesh) return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight) {
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	} else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	} else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void OverworldScene::RenderText(Mesh* mesh, std::string text, Color color) {
	if (!mesh || mesh->textureID <= 0) return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i) {
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

void OverworldScene::RoadTeleport() {
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (camera.position.x >= 39.3 && 
		camera.position.x <= 74.3 && 
		camera.position.z <= 100 && 
		camera.position.z >= 80) {
		if (currentCarObject) {
			Application::sceneswitch = Application::SCENEBEACH;
			Application::previousscene = Application::OVERWORLD;
			camera.position.z = 70;
		} else {
			RenderTextOnScreen(meshList[GEO_TEXT], "You need to be in a vehicle to go here!", Colors::WHITE, 4, 0 * w / 800, 10 * h / 600);
		}
	}
	else if (camera.position.x <= -36.4 &&
		camera.position.x >= -52 &&
		camera.position.z <= 100 &&
		camera.position.z >= 80) {
		if (currentCarObject) {
			Application::sceneswitch = Application::SCENESHAQ;
			Application::previousscene = Application::OVERWORLD;
			camera.position.z = 70;
		} else {
			RenderTextOnScreen(meshList[GEO_TEXT], "You need to be in a vehicle to go here!", Colors::WHITE, 4, 0 * w / 800, 10 * h / 600);
		}
	}
}

void OverworldScene::RenderTeleportText() {
	modelStack.PushMatrix();
	modelStack.Translate(74.3, 5, 100);
	modelStack.Rotate(180, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "This way to the beach!", Colors::WHITE);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-36.4, 4, 100);
	modelStack.Rotate(180, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "This way to the mart!", Colors::WHITE);
	modelStack.PopMatrix();
}

void OverworldScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
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
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i) {
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + (i * 0.7f), 0.5f, 0);
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

void OverworldScene::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

void OverworldScene::Update(double dt, Mouse mouse) {
	if (Application::IsKeyPressedOnce(VK_ESCAPE)) {
		Application::sceneswitch = Application::STARTSCENE;
	}

	if (Application::IsKeyPressedOnce('G')) {
		showTaskbar = showTaskbar ? 0 : 1;
	}

	meshList[MOON]->transform.translate = camera.position;
	meshList[MOON]->transform.translate.y = camera.position.y + (2 * camera.bounds);
	
	if (Application::previousscene != Application::OVERWORLD) {
		InitGL();
	}

	light[0].position.Set(meshList[MOON]->transform.translate.x, meshList[MOON]->transform.translate.y, meshList[MOON]->transform.translate.z);

	if (!currentCarObject) {
		camera.Update(dt, mouse);
	} else {
		camera.UpdateCar(dt, mouse, 6.f);
	}

	RoadTeleport();
	DetectCollision();
	GetInCar();
	CompleteTasks();
	UpdateRobo(dt);
	UpdatePlanes(dt);
}

void OverworldScene::InitGL() {
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
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
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

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	Mesh::SetMaterialLoc(
		m_parameters[U_MATERIAL_AMBIENT], 
		m_parameters[U_MATERIAL_DIFFUSE], 
		m_parameters[U_MATERIAL_SPECULAR], 
		m_parameters[U_MATERIAL_SHININESS]
	);

	light[0].color = Colors::WHITE;
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(60));
	light[0].cosInner = cos(Math::DegreeToRadian(45));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
}

void OverworldScene::InitGLXray() {
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
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
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

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	Mesh::SetMaterialLoc(
		m_parameters[U_MATERIAL_AMBIENT], 
		m_parameters[U_MATERIAL_DIFFUSE], 
		m_parameters[U_MATERIAL_SPECULAR], 
		m_parameters[U_MATERIAL_SHININESS]
	);

	light[0].color = Colors::WHITE;
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(60));
	light[0].cosInner = cos(Math::DegreeToRadian(45));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
}

void OverworldScene::Update(double dt) {
	Mouse mouse;
	Update(dt, mouse);
}

void OverworldScene::RenderSkybox() {
	float translate = 2 * camera.bounds;
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

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_GROUND], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[MOON]->transform.translate.x, meshList[MOON]->transform.translate.y, meshList[MOON]->transform.translate.z);
	modelStack.Rotate(meshList[MOON]->transform.rotate, 0, 1, 0);
	modelStack.Scale(meshList[MOON]->transform.scale.x, meshList[MOON]->transform.scale.y, meshList[MOON]->transform.scale.z);
	RenderMesh(meshList[MOON], false);
	modelStack.PopMatrix();
}

void OverworldScene::RenderObjects() {
	for (auto car : sceneManager->allObjects) {
		if (car->mesh) {
			modelStack.PushMatrix();
			modelStack.Translate(car->transform->translate.x, car->transform->translate.y, car->transform->translate.z);
			modelStack.Rotate(car->transform->rotate, 0, 1, 0);
			modelStack.Scale(car->transform->scale.x, car->transform->scale.y, car->transform->scale.z);
			RenderMesh(car->mesh, true);
			modelStack.PopMatrix();
		}
	}
}

void OverworldScene::RenderTasks() {
	if (showTaskbar) {
		bool allComplete = true;
		if (showTaskbarFrame < 30) {
			showTaskbarFrame++;
		}
		RenderMeshOnScreen(meshList[SIDEBAR], 40, (80 + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800), 40 * (Application::GetWindowHeight() / 600));
		RenderTextOnScreen(meshList[GEO_TEXT], "Tasks", Colors::BLUE, 3, (21.8f + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800), 15.5f * (Application::GetWindowHeight() / 600));
		for (unsigned i = 0; i < NUM_TASKS; i++) {
			Color completed = Colors::RED;
			if (tasks[i]) {
				completed = Colors::DARK_GREEN;
			}
			if (!tasks[i]) allComplete = false;
			float x = (30.5f + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800);
			float y = (21.5f - i) * (Application::GetWindowHeight() / 600);
			switch (i) {
			case STEAL_CAR:
				RenderTextOnScreen(meshList[GEO_TEXT], "Steal Vehicle", completed, 2, x, y);
				break;
			case ENTER_BUILDING:
				RenderTextOnScreen(meshList[GEO_TEXT], "Enter Building", completed, 2, x, y);
				break;
			case SPRINT:
				RenderTextOnScreen(meshList[GEO_TEXT], "LShift Sprint", completed, 2, x, y);
				break;
			case WALK:
				RenderTextOnScreen(meshList[GEO_TEXT], "WASD Walking", completed, 2, x, y);
				break;
			case LOOK:
				RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Control", completed, 2, x, y);
				break;
			case JUMP:
				RenderTextOnScreen(meshList[GEO_TEXT], "Space Jumping", completed, 2, x, y);
				break;
			default:
				break;
			}
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ".", allComplete ? Colors::DARK_GREEN : Colors::WHITE, 1, 0, 0);
	} else {
		if (showTaskbarFrame != 0) {
			bool allComplete = true;
			if (showTaskbarFrame > 0) {
				showTaskbarFrame--;
			}
			RenderMeshOnScreen(meshList[SIDEBAR], 40, (80 + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800), 40 * (Application::GetWindowHeight() / 600));
			RenderTextOnScreen(meshList[GEO_TEXT], "Tasks", Colors::BLUE, 3, (21.8f + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800), 15.5f * (Application::GetWindowHeight() / 600));
			for (unsigned i = 0; i < NUM_TASKS; i++) {
				Color completed = Colors::RED;
				if (tasks[i]) {
					completed = Colors::DARK_GREEN;
				}
				if (!tasks[i]) allComplete = false;
				float x = (30.5f + (30 - showTaskbarFrame)) * (Application::GetWindowWidth() / 800);
				float y = (21.5f - i) * (Application::GetWindowHeight() / 600);
				switch (i) {
				case STEAL_CAR:
					RenderTextOnScreen(meshList[GEO_TEXT], "Steal Vehicle", completed, 2, x, y);
					break;
				case ENTER_BUILDING:
					RenderTextOnScreen(meshList[GEO_TEXT], "Enter Building", completed, 2, x, y);
					break;
				case SPRINT:
					RenderTextOnScreen(meshList[GEO_TEXT], "LShift Sprint", completed, 2, x, y);
					break;
				case WALK:
					RenderTextOnScreen(meshList[GEO_TEXT], "WASD Walking", completed, 2, x, y);
					break;
				case LOOK:
					RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Control", completed, 2, x, y);
					break;
				case JUMP:
					RenderTextOnScreen(meshList[GEO_TEXT], "Space Jumping", completed, 2, x, y);
					break;
				default:
					break;
				}
			}
			RenderTextOnScreen(meshList[GEO_TEXT], ".", allComplete ? Colors::DARK_GREEN : Colors::WHITE, 1, 0, 0);
		}
	}
}

void OverworldScene::CompleteTasks() {
	if (!tasks[WALK]) {
		if (camera.position != camera.defaultPosition) {
			tasks[WALK] = 1;
			showTaskbar = 1;
		}
	}
	if (!tasks[SPRINT]) {
		if (Application::IsKeyPressed(VK_LSHIFT) && Application::IsKeyPressed('W')) {
			tasks[SPRINT] = 1;
			showTaskbar = 1;
		}
	}
	if (!tasks[LOOK]) {
		Vector3 defaultView = (camera.defaultTarget - camera.defaultPosition).Normalized();
		Vector3 currentView = (camera.target - camera.position).Normalized();
		if (defaultView != currentView) {
			tasks[LOOK] = 1;
			showTaskbar = 1;
		}
	}
	if (!tasks[STEAL_CAR]) {
		if (currentCar) {
			tasks[STEAL_CAR] = 1;
			showTaskbar = 1;
		}
	}
	if (!tasks[ENTER_BUILDING]) {
		//showTaskbar = 1;
	}
	if (!tasks[JUMP]) {
		if (Application::IsKeyPressedOnce(' ')) {
			tasks[JUMP] = 1;
			showTaskbar = 1;
		}
	}
	if (showTaskbar && showTaskbarFrame == 30) {
		showTaskbarDelay++;
	}
	if (showTaskbarDelay > Application::GetFPS() * 5) {
		showTaskbar = showTaskbarDelay = 0;
	}
}

void OverworldScene::RenderUI() {
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	RenderMeshOnScreen(meshList[GEO_UI], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(Player::getHealth()), Colors::BLACK, 2, 0.5 * w / 800, 28.5 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:" + std::to_string(Player::getAmmo()), Colors::BLACK, 2, 0.5 * w / 800, 27 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:" + std::to_string(Player::getMoney()), Colors::BLACK, 2, 0.5 * w / 800, 25.5 * h / 600);
}

void OverworldScene::RenderRobo() {
	modelStack.PushMatrix();

	modelStack.Translate(-32.2, 2.5, translateSphereZ);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
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

	modelStack.Translate(38.2, 2.5, translateSphereZ2);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// 3d robot
	modelStack.PushMatrix();

	modelStack.Translate(76.2, 2.5, translateSphereZ);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// 4th robot
	modelStack.PushMatrix();

	modelStack.Translate(-74.2, 2.5, translateSphereZ2);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// 5th robot
	modelStack.PushMatrix();

	modelStack.Translate(translateSphereX1, 2.5, -79);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -8.5, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	// 6th robot
	modelStack.PushMatrix();

	modelStack.Translate(translateSphereX2, 2.5, 69.6);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.3, 0.27, 0.3);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBORIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.25, 4.5, 0);
	modelStack.Rotate(-rotateleftleg, 1, 0, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_ROBOLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 4.5, 0);
	modelStack.Rotate(rotateleftleg, 1, 0, 0);
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

void OverworldScene::RenderSpeedometer() {
	RenderMeshOnScreen(meshList[GEO_UI], 4, 5, 0);
	std::stringstream speedStream;
	speedStream << camera.currentCarSpeed * 10;
	speedStream.precision(3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Speed: " + speedStream.str(), Colors::WHITE, 4, 5, 0);
}

void OverworldScene::GetInCar() {
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (!currentCarObject) {
		for (auto object : sceneManager->allObjects) {
			if (object->type == GameObject::CAR) {
				if (!object->camera && isNearObject(object, 3.f)) {
					RenderTextOnScreen(meshList[GEO_TEXT], "Press F to get in Car", Colors::WHITE, 4, 3 * w / 800, 4 * h / 600);
					if (Application::IsKeyPressedOnce('F')) {
						currentCarObject = object;
						camera.currentCarSpeed = 0;
						camera.rightY = 0;
						camera.position.x = object->transform->translate.x;
						camera.position.z = object->transform->translate.z;
						tasks[STEAL_CAR] = 1;
						if (object->target != carOrigin) {
							camera.carTarget = object->target;
							camera.target = object->target;
						} else {
							camera.carTarget = camera.position + carOrigin;
							camera.target = camera.position + carOrigin;
						}
						break;
					}
				}
			}
		}
	}

	if (currentCarObject) {
		float carY = currentCarObject->transform->translate.y;
		currentCarObject->transform->Translate(camera.position.x, carY, camera.position.z);
		camera.position.y = carY + 5;

		currentCarObject->transform->RotateDegree(camera.getCarRotation(carOrigin));
		currentCarObject->target = camera.carTarget;

		if (Application::IsKeyPressedOnce('F')) {
			Vector3 view = (camera.target - camera.position).Normalized();
			Vector3 right = view.Cross(Vector3(0, 1, 0)).Normalized();
			while (true) {
				if (isNearObject(currentCarObject, 2)) {
					camera.position.x += -(2 * right.x);
					camera.position.z += -(2 * right.z);
				} else {
					break;
				}
			}
			camera.position.y = camera.defaultPosition.y;
			camera.target.y = camera.defaultPosition.y;
			camera.orthographic_size = camera.prevFOV;
			currentCarObject = nullptr;
		}
	}
}

void OverworldScene::DetectCollision() {
	for (auto o : sceneManager->allObjects) {
		if (o == currentCarObject) continue;
		if (isNearObject(o, (o->transform->scale.x > 1 ? 2 : 2.5 * o->transform->scale.x))) {
			MoveBack();
		}
	}
}

void OverworldScene::UpdateRobo(double &dt) {
	translateSphereZ += (float)(2.5 * dt);
	translateSphereZ2 -= (float)(2.75 * dt);
	translateSphereX1 -= (float)(2.5 * dt);
	translateSphereX2 += (float)(2.75 * dt);

	if (leftleglimit == true) {
		rotateleftleg += 1;
		if (rotateleftleg > 30)
			leftleglimit = false;

	} else if (leftleglimit == false) {
		rotateleftleg -= 1;
		if (rotateleftleg < -30)
			leftleglimit = true;
	}

	if (translateSphereZ >= 30.5)
		translateSphereZ = -39;

	if (translateSphereZ2 <= -38)
		translateSphereZ2 = 29;

	if (translateSphereX1 <= -33.4)
		translateSphereX1 = 36.4;

	if (translateSphereX2 >= 34.2)
		translateSphereX2 = -30;
}

void OverworldScene::UpdatePlanes(double& dt) {
	for (auto p : sceneManager->allObjects) {
		if (p && p->mesh->name == "plane") {
			if (p->transform->translate.x > 2 * camera.bounds) {
				p->transform->translate.x = -2 * camera.bounds;
			}
			if (p->transform->translate.x < -2 * camera.bounds) {
				p->transform->translate.x = 2 * camera.bounds;
			}
			if (p->transform->translate.z > 2 * camera.bounds) {
				p->transform->translate.z = -2 * camera.bounds;
			}
			if (p->transform->translate.z < -2 * camera.bounds) {
				p->transform->translate.z = 2 * camera.bounds;
			}
			p->transform->Translate(
				p->transform->translate.x + (p->target.x), 
				p->transform->translate.y + (p->target.y), 
				p->transform->translate.z + (p->target.z)
			);
		}
	}
}

void OverworldScene::CreateCityObjects() {
	for (int i = -4; i <= 4; i+=2) {
		for (int j = -4; j <= 4; j+=2) {
			if (i == 0 || j == 0) {
				continue;
			}
			GameObject* object = new GameObject(meshList[STREETLIGHT]);
			object->transform->Translate(i * 19, 0.5f, j * 18);
			sceneManager->push(object);
			object->id = sceneManager->totalObjects;
		}
	}

	GameObject* building = new GameObject(meshList[SKYSCRAPER2]);
	building->transform->Translate(0, 1, 0);
	building->transform->Scale(2, 5, 2);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	///////////////////////////////////////////////

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(115, 0, -25);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(115, 0, -10);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(115, 0, 5);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(115, 0, 20);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	///////////////////////////////////////////////

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, -25);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, -10);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, 5);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, 20);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	///////////////////////////////////////////////

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, -25);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, -10);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, 5);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-115, 0, 20);
	building->transform->RotateDegree(180);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	///////////////////////////////////////////////

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-25, 0, 115);
	building->transform->RotateDegree(-90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-10, 0, 115);
	building->transform->RotateDegree(-90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(5, 0, 115);
	building->transform->RotateDegree(-90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(20, 0, 115);
	building->transform->RotateDegree(-90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	///////////////////////////////////////////////

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-25, 0, -115);
	building->transform->RotateDegree(90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(-10, 0, -115);
	building->transform->RotateDegree(90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(5, 0, -115);
	building->transform->RotateDegree(90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;

	building = new GameObject(meshList[NY_BUILDING]);
	building->transform->Translate(20, 0, -115);
	building->transform->RotateDegree(90);
	building->transform->Scale(0.6);
	sceneManager->push(building);
	building->id = sceneManager->totalObjects;
}

void OverworldScene::MoveBack() {

	// Get the current view vector and current y position
	Vector3 view = (camera.target - camera.position).Normalized();
	float y = camera.position.y;

	// Set the player back to previous position but current y position (only x & z collision)
	camera.position = camera.prevPosition;
	camera.position.y = y;

	// Set the correct target according to player's position and set the car speed to 0
	camera.target = camera.position + view;
	camera.currentCarSpeed = 0;
}

bool OverworldScene::isNear(Mesh* mesh, const float& distance) {
	if (mesh->type == Mesh::TYPE::OBJECT) {

		// Get distance between object and camera
		float d = Math::Square(mesh->transform.translate.x - camera.position.x) + Math::Square(mesh->transform.translate.z - camera.position.z);
		return (d - (2 * distance)) <= 0;
	}
	return false;
}

bool OverworldScene::isNearObject(GameObject* o, const float& distance) {
	if (o->mesh->type == Mesh::TYPE::OBJECT) {

		// Get distance between object and camera
		float d = Math::Square(o->transform->translate.x - camera.position.x) + Math::Square(o->transform->translate.z - camera.position.z);
		return (d - (2 * distance)) <= 0;
	}
	return false;
}

bool OverworldScene::isHit(GameObject* o1, GameObject* o2, const float& distance) {
	if (o1->mesh->type == Mesh::TYPE::OBJECT && o2->mesh->type == Mesh::TYPE::OBJECT) {

		// Get distance between two objects
		float d = Math::Square(o1->transform->translate.x - o2->transform->translate.x) + Math::Square(o1->transform->translate.z - o2->transform->translate.z);
		return (d - (2 * distance)) <= 0;
	}
	return false;
}

void OverworldScene::Render() {
	//Clear the color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	RenderObjects();
	RenderTasks();
	RenderRobo();
	RenderTeleportText();
	RenderUI();
	if (currentCarObject) {
		RenderSpeedometer();
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 1, 0, 0);
}

void OverworldScene::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	delete sceneManager;
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void OverworldScene::Reset() {
	if (sceneManager) {
		delete sceneManager;
	}
	sceneManager = new SceneManager(this, camera.bounds);
	Mtx44 projection;
	projection.SetToPerspective(45.f, Application::GetWindowWidth() / Application::GetWindowHeight(), 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(18, 3, 0), Vector3(5, 3, 1), Vector3(0, 1, 0), (float)100);
	currentCar = nullptr;
	currentCarObject = nullptr;

	meshList[MOON]->transform.RotateDegree(0);
	meshList[MOON]->transform.Scale(10.f);

	light[0].type = Light::LIGHT_SPOT;

	meshList[CAR2]->transform.Translate(-22, 1.8f, 65);
	meshList[CAR2]->transform.RotateDegree(0);
	meshList[CAR2]->transform.Scale(0.15);
	meshList[CAR2]->corner[Mesh::CORNER::C1] = meshList[CAR2]->transform.translate + Vector3(1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C2] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);
	meshList[CAR2]->corner[Mesh::CORNER::C3] = meshList[CAR2]->transform.translate + Vector3(-1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C4] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);
	GameObject* car = new GameObject(meshList[CAR2], meshList[CAR2]->transform);
	car->type = GameObject::CAR;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	meshList[BUS1]->transform.Translate(0, 4.5f, 55);
	meshList[BUS1]->transform.RotateDegree(0);
	meshList[BUS1]->transform.Scale(0.5);
	meshList[BUS1]->corner[Mesh::CORNER::C1] = meshList[BUS1]->transform.translate + Vector3(1, 0, -1);
	meshList[BUS1]->corner[Mesh::CORNER::C2] = meshList[BUS1]->transform.translate + Vector3(1, 0, 1);
	meshList[BUS1]->corner[Mesh::CORNER::C3] = meshList[BUS1]->transform.translate + Vector3(-1, 0, -1);
	meshList[BUS1]->corner[Mesh::CORNER::C4] = meshList[BUS1]->transform.translate + Vector3(1, 0, 1);
	car = new GameObject(meshList[BUS1], meshList[BUS1]->transform);
	car->type = GameObject::CAR;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	meshList[CAR1]->transform.Translate(-10, 2.3f, 65);
	meshList[CAR1]->transform.RotateDegree(0);
	meshList[CAR1]->transform.Scale(0.15);
	meshList[CAR1]->corner[Mesh::CORNER::C1] = meshList[CAR1]->transform.translate + Vector3(1, 0, -1);
	meshList[CAR1]->corner[Mesh::CORNER::C2] = meshList[CAR1]->transform.translate + Vector3(1, 0, 1);
	meshList[CAR1]->corner[Mesh::CORNER::C3] = meshList[CAR1]->transform.translate + Vector3(-1, 0, -1);
	meshList[CAR1]->corner[Mesh::CORNER::C4] = meshList[CAR1]->transform.translate + Vector3(1, 0, 1);
	car = new GameObject(meshList[CAR1], meshList[CAR1]->transform);
	car->type = GameObject::CAR;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	meshList[TRUCK2]->transform.Translate(-40, 5.3f, 65);
	meshList[TRUCK2]->transform.RotateDegree(0);
	meshList[TRUCK2]->transform.Scale(0.13);
	car = new GameObject(meshList[TRUCK2], meshList[TRUCK2]->transform);
	car->type = GameObject::CAR;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	meshList[TRUCK1]->transform.Translate(10, 5.1f, 63);
	meshList[TRUCK1]->transform.RotateDegree(0);
	meshList[TRUCK1]->transform.Scale(0.17);
	meshList[TRUCK1]->corner[Mesh::CORNER::C1] = meshList[TRUCK1]->transform.translate + Vector3(1, 0, -1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C2] = meshList[TRUCK1]->transform.translate + Vector3(-1, 0, 1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C3] = meshList[TRUCK1]->transform.translate + Vector3(1, 0, 1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C4] = meshList[TRUCK1]->transform.translate + Vector3(-1, 0, -1);

	car = new GameObject(meshList[TRUCK1], meshList[TRUCK1]->transform);
	car->type = GameObject::CAR;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	car = new GameObject(meshList[PLANE]);
	car->transform->Translate(camera.bounds, 50, camera.bounds);
	car->type = GameObject::UNKNOWN;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;
	car->target = Vector3(0, 0, 1);

	car = new GameObject(meshList[PLANE]);
	car->transform->Translate(camera.bounds, 60, -camera.bounds);
	car->transform->RotateDegree(90);
	car->type = GameObject::UNKNOWN;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;
	car->target = Vector3(1, 0, 0);

	car = new GameObject(meshList[PLANE]);
	car->transform->Translate(camera.bounds, 70, camera.bounds);
	car->transform->RotateDegree(270);
	car->type = GameObject::UNKNOWN;
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;
	car->target = Vector3(-1, 0, 0);

	CreateCityObjects();

	GameObject *cameraObject = new GameObject(meshList[CAMERA]);
	cameraObject->transform->translate = camera.position;
	sceneManager->push(cameraObject);
	cameraObject->id = sceneManager->totalObjects;
	cameraObject->camera = 1;

	for (unsigned i = 0; i < NUM_TASKS; i++) {
		tasks[i] = 0;
	}

	showTaskbar = false;
	showTaskbarFrame = showTaskbarDelay =0;
	carOrigin = Vector3(-1, 0, 0);

	rotateleftleg = 0;
	leftleglimit = false;
	translateSphereZ = -19.6;
	translateSphereZ2 = 19.6;
	translateSphereX1 = 0;
	translateSphereX2 = 0;
}