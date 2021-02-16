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

void OverworldScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //bg colour

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
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(5, 2, 5), Vector3(1, 2, 1), Vector3(0, 1, 0), (float)100);
	currentCar = nullptr;

	//shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] =
		glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] =
		glGetUniformLocation(m_programID, "colorTexture");

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
	glUseProgram(m_programID);
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	//Replace previous code
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 50, 0);
	light[0].color = WHITE;
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(0, 50, 0);
	light[1].color = WHITE;
	light[1].power = 0.5f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

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

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 10, 10, 10);

	meshList[GEO_FRONT] = MeshBuilder::GenerateSkybox("front", WHITE, 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front-space.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateSkybox("back", WHITE, 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back-space.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateSkybox("left", WHITE, 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//right-space.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateSkybox("right", WHITE, 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//left-space.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateSkybox("top", WHITE, 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top-space.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateSkybox("bottom", WHITE, 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom-space.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_GROUND] = MeshBuilder::GenerateGround("ground", GRAY, 1000.f, 18);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//roadcross.tga");

	meshList[TRUCK1] = MeshBuilder::GenerateOBJ("truck1", "OBJ//NewTruck2.obj");
	meshList[TRUCK1]->textureID = LoadTGA("Image//Vehicle_Silver.tga");
	meshList[TRUCK1]->transform.Translate(10, 2.f, 65);
	meshList[TRUCK1]->transform.Scale(0.04);
	meshList[TRUCK1]->corner[Mesh::CORNER::C1] = meshList[TRUCK1]->transform.translate + Vector3(1, 0, -1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C2] = meshList[TRUCK1]->transform.translate + Vector3(-1, 0, 1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C3] = meshList[TRUCK1]->transform.translate + Vector3(1, 0, 1);
	meshList[TRUCK1]->corner[Mesh::CORNER::C4] = meshList[TRUCK1]->transform.translate + Vector3(-1, 0, -1);

	meshList[CAR1] = MeshBuilder::GenerateOBJ("car1", "OBJ//NewCar1.obj");
	meshList[CAR1]->textureID = LoadTGA("Image//GreenVehicle.tga");
	meshList[CAR1]->transform.Translate(-10, 1.5f, 65);
	meshList[CAR1]->transform.Scale(0.1);
	meshList[CAR1]->corner[Mesh::CORNER::C1] = meshList[CAR1]->transform.translate + Vector3(1, 0, -1);
	meshList[CAR1]->corner[Mesh::CORNER::C2] = meshList[CAR1]->transform.translate + Vector3(1, 0, 1);
	meshList[CAR1]->corner[Mesh::CORNER::C3] = meshList[CAR1]->transform.translate + Vector3(-1, 0, -1);
	meshList[CAR1]->corner[Mesh::CORNER::C4] = meshList[CAR1]->transform.translate + Vector3(1, 0, 1);

	meshList[BUS1] = MeshBuilder::GenerateOBJ("bus", "OBJ//bus.obj");
	meshList[BUS1]->textureID = LoadTGA("Image//Vehicle_Silver.tga");
	meshList[BUS1]->transform.Translate(0, 3.6f, 55);
	meshList[BUS1]->transform.Scale(0.4);
	meshList[BUS1]->corner[Mesh::CORNER::C1] = meshList[BUS1]->transform.translate + Vector3(1, 0, -1);
	meshList[BUS1]->corner[Mesh::CORNER::C2] = meshList[BUS1]->transform.translate + Vector3(1, 0, 1);
	meshList[BUS1]->corner[Mesh::CORNER::C3] = meshList[BUS1]->transform.translate + Vector3(-1, 0, -1);
	meshList[BUS1]->corner[Mesh::CORNER::C4] = meshList[BUS1]->transform.translate + Vector3(1, 0, 1);

	meshList[CAR2] = MeshBuilder::GenerateOBJ("Car2", "OBJ//NewCar2.obj");
	meshList[CAR2]->textureID = LoadTGA("Image//RedVehicle.tga");
	meshList[CAR2]->transform.Translate(-20, 1.2f, 65);
	meshList[CAR2]->transform.Scale(0.1);
	meshList[CAR2]->corner[Mesh::CORNER::C1] = meshList[CAR2]->transform.translate + Vector3(1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C2] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);
	meshList[CAR2]->corner[Mesh::CORNER::C3] = meshList[CAR2]->transform.translate + Vector3(-1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C4] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);

	meshList[SKYSCRAPER2] = MeshBuilder::GenerateOBJ("skyscraper", "OBJ//skyscraper4.obj");
	meshList[SKYSCRAPER2]->transform.Scale(5);
}

void OverworldScene::RenderMesh(Mesh* mesh, bool enableLight)
{
	if (!mesh) return;
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

void OverworldScene::RenderText(Mesh* mesh, std::string text, Color color)
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

void OverworldScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
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

void OverworldScene::RenderMeshOnScreen(Mesh* mesh, Color color, float size, float x, float y) {
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

void OverworldScene::Update(double dt, Mouse mouse) {
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);

	else if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode

	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (!currentCar) {
		camera.Update(dt, mouse);
		DetectCollision();
	} else {
		camera.UpdateCar(dt, mouse, (float)3.f);
	}
}

void OverworldScene::Update(double dt) {
	Mouse mouse;
	Update(dt, mouse);
}

void OverworldScene::RenderSkybox() {
	float translate = 100;
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
}

void OverworldScene::RenderVehicles() {
	for (unsigned car = TRUCK1; car < NUM_CAR; car++) {
		if (meshList[car]) {
			meshList[car]->corner[Mesh::CORNER::C1] += meshList[car]->transform.translate;
			meshList[car]->corner[Mesh::CORNER::C2] += meshList[car]->transform.translate;
			meshList[car]->corner[Mesh::CORNER::C3] += meshList[car]->transform.translate;
			meshList[car]->corner[Mesh::CORNER::C4] += meshList[car]->transform.translate;
			modelStack.PushMatrix();
			modelStack.Translate(meshList[car]->transform.translate.x, meshList[car]->transform.translate.y, meshList[car]->transform.translate.z);
			modelStack.Rotate(meshList[car]->transform.rotate, 0, 1, 0);
			modelStack.Scale(meshList[car]->transform.scale.x, meshList[car]->transform.scale.y, meshList[car]->transform.scale.z);
			RenderMesh(meshList[car], true);
			modelStack.PopMatrix();
		}
	}

	GetInCar();
}

void OverworldScene::RenderBuildings() {
	for (unsigned building = SKYSCRAPER1; building < NUM_BUILDINGS; building++) {
		if (meshList[building]) {
			modelStack.PushMatrix();
			modelStack.Translate(meshList[building]->transform.translate.x, meshList[building]->transform.translate.y, meshList[building]->transform.translate.z);
			modelStack.Rotate(meshList[building]->transform.rotate, 0, 1, 0);
			modelStack.Scale(meshList[building]->transform.scale.x, meshList[building]->transform.scale.y, meshList[building]->transform.scale.z);
			RenderMesh(meshList[SKYSCRAPER2], true);
			modelStack.PopMatrix();
		}
	}
}

void OverworldScene::GetInCar() {
	if (!currentCar) {
		for (unsigned car = TRUCK1; car < NUM_CAR; car++) {
			if (meshList[car]) {
				if (isNear(meshList[car], (float)3.f)) {
					RenderTextOnScreen(meshList[GEO_TEXT], "Press F to get in Car", WHITE, 4, 3, 7);
					if (Application::IsKeyPressedOnce('F')) {
						currentCar = meshList[car];
						camera.position.x = currentCar->transform.translate.x;
						camera.position.z = currentCar->transform.translate.z;
						switch (car) {
						default:
							carOrigin = Vector3(-1, 0, 0);
							break;
						}
						camera.carTarget = camera.position + carOrigin;
						camera.target = camera.position + carOrigin;
						break;
					}
				}
			}
		}
	}
	if (currentCar) {
		currentCar->prevTransform = currentCar->transform;
		currentCar->transform.translate.x = camera.position.x;
		currentCar->transform.translate.z = camera.position.z;
		camera.position.y = currentCar->transform.translate.y + 5;
		Vector3 origin = (currentCar->transform.translate + carOrigin).Normalized();
		currentCar->transform.rotate = camera.getCarRotation(carOrigin);
		if (Application::IsKeyPressedOnce('F')) {
			Vector3 view = (camera.target - camera.position).Normalized();
			Vector3 right = view.Cross(Vector3(0, 1, 0)).Normalized();
			while (true) {
				if (isNear(currentCar, 2)) {
					camera.position.x += - (2 * right.x);
					camera.position.z += - (2 * right.z);
				} else {
					break;
				}
			}
			camera.position.y = camera.defaultPosition.y;
			camera.target.y = camera.defaultPosition.y;
			camera.orthographic_size = camera.prevFOV;
			currentCar = nullptr;
		}
	}
}

void OverworldScene::DetectCollision() {
	for (unsigned object = 1; object < NUM_GEOMETRY; object++) {
		if (meshList[object] && meshList[object - 1]) {
			if (isHit(meshList[object - 1], meshList[object], (meshList[object]->transform.scale.x > 1 ? 2.f : meshList[object]->transform.scale.x))) {
				//ObjectMoveBack(meshList[object - 1]);
			}
		}
	}
	if (!currentCar) {
		for (unsigned object = 0; object < NUM_GEOMETRY; object++) {
			if (meshList[object]) {
				if (isNear(meshList[object], 2.f)) {
					MoveBack();
				}
			}
		}
	}
}

void OverworldScene::MoveBack() {
	camera.target = camera.prevTarget;
	camera.position = camera.prevPosition;
	camera.up = camera.prevUp;
}

void OverworldScene::ObjectMoveBack(Mesh* mesh) {
	mesh->transform = mesh->prevTransform;
}

bool OverworldScene::isNear(Mesh* mesh, const float& distance) {
	if (mesh->type == Mesh::TYPE::OBJECT) {
		return (camera.position.x <= (mesh->transform.translate.x + distance) &&
			camera.position.x >= (mesh->transform.translate.x - distance)) &&
			(camera.position.z <= (mesh->transform.translate.z + distance) &&
			camera.position.z >= (mesh->transform.translate.z - distance));
	}
	return false;
}

bool OverworldScene::isHit(Mesh* mesh1, Mesh* mesh2, const float& distance) {
	if (mesh1->type == Mesh::TYPE::OBJECT && mesh2->type == Mesh::TYPE::OBJECT) {
		return (mesh1->transform.translate.x <= (mesh2->transform.translate.x + distance) &&
			mesh1->transform.translate.x >= (mesh2->transform.translate.x - distance)) &&
			(mesh1->transform.translate.z <= (mesh2->transform.translate.z + distance) &&
			mesh1->transform.translate.z >= (mesh2->transform.translate.z - distance));
	}
	return false;
}

void OverworldScene::Render()
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
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	RenderSkybox();
	RenderBuildings();
	RenderVehicles();
}

void OverworldScene::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}