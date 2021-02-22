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

	Mesh::SetMaterialLoc(
		m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE], 
		m_parameters[U_MATERIAL_SPECULAR], 
		m_parameters[U_MATERIAL_SHININESS]);

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

	meshList[MOON] = MeshBuilder::GenerateSphere("moon", Colors::GRAY, 30, 30, 1.f);
	meshList[MOON]->type = Mesh::TYPE::IMAGE;

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->type = Mesh::TYPE::IMAGE;

	meshList[SIDEBAR] = MeshBuilder::GenerateFaceQuad("sidebar", Colors::GRAY, 1.f, 1.f);
	meshList[SIDEBAR]->textureID = LoadTGA("Image//button.tga");
	meshList[SIDEBAR]->type = Mesh::TYPE::IMAGE;

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
	meshList[SKYSCRAPER2]->transform.Scale(5);
	meshList[SKYSCRAPER2]->type = Mesh::TYPE::OBJECT;

	meshList[STREETLIGHT] = MeshBuilder::GenerateOBJ("streetlight", "OBJ//StreetLight.obj");
	meshList[STREETLIGHT]->textureID = LoadTGA("Image//StreetLight.tga");
	meshList[STREETLIGHT]->type = Mesh::TYPE::OBJECT;

	meshList[CAMERA] = new Mesh("camera");
	meshList[CAMERA]->type = Mesh::TYPE::CAMERA;

	Reset();

	// Init light
	light[0].color = Colors::WHITE;
	light[0].power = 0.7;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].color = Colors::WHITE;
	light[1].power = 1.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

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
	if (Application::IsKeyPressedOnce('R')) {
		Reset();
	}

	if (Application::IsKeyPressedOnce(VK_ESCAPE)) {
		Application::sceneswitch = Application::STARTSCENE;
	}

	if (Application::IsKeyPressedOnce('G')) {
		showTaskbar = showTaskbar ? 0 : 1;
	}

	light[0].position.Set(meshList[MOON]->transform.translate.x, meshList[MOON]->transform.translate.y, meshList[MOON]->transform.translate.z);

	if (!currentCarObject) {
		camera.Update(dt, mouse);
	} else {
		camera.UpdateCar(dt, mouse, (float)6.f);
		Application::log("right after playerx: " + std::to_string(camera.position.x));
	}
	sceneManager->split(sceneManager->root);

	DetectCollision();
	GetInCar();
	CompleteTasks();

	sceneManager->deleteAllQuad(sceneManager->root);
	sceneManager->root = new Quad(camera.bounds);
	for (auto o : sceneManager->allObjects) {
		sceneManager->root->gameObjects.push_back(o);
	}
	sceneManager->root->count = sceneManager->allObjects.size();
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

	meshList[MOON]->transform.translate = camera.position;
	meshList[MOON]->transform.translate.y = camera.position.y + translate;

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

void OverworldScene::GetInCar() {
	if (!currentCarObject) {
		Quad* quad = sceneManager->getQuad(sceneManager->totalObjects);
		if (quad) {
			for (auto object : quad->gameObjects) {
				if (isNearObject(object, 3.f)) {
					RenderTextOnScreen(meshList[GEO_TEXT], "Press F to get in Car", Colors::WHITE, 4, 3, 4);
					if (Application::IsKeyPressedOnce('F')) {
						currentCarObject = object;
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
		} else {
			Application::log("no quad");
		}
	}

	if (currentCarObject) {
		float carY = currentCarObject->transform->translate.y;
		currentCarObject->transform->Translate(camera.position.x, carY, camera.position.z);
		Application::log("carx: " + std::to_string(currentCarObject->transform->translate.x));
		Application::log("playerx: " + std::to_string(camera.position.x));
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
	Quad* quad = sceneManager->getQuad(sceneManager->totalObjects);
	if (quad) {
		for (auto o : quad->gameObjects) {
			if (isNearObject(o, 2)) {
				MoveBack();
			}
		}
	}
}

void OverworldScene::CreateCityObjects() {
	for (int i = -4; i < 4; i+=2) {
		for (int j = -4; j < 4; j+=2) {
			if (i == 0 || j == 0) {
				continue;
			}
			GameObject* object = new GameObject(meshList[STREETLIGHT]);
			object->transform->Translate(i * 10, 0.5f, j * 10);
			sceneManager->push(object);
			object->id = sceneManager->totalObjects;
		}
	}
}

void OverworldScene::MoveBack() {
	Vector3 view = (camera.target - camera.position).Normalized();
	float y = camera.position.y;
	camera.position = camera.prevPosition;
	camera.position.y = y;
	camera.target = camera.position + view;
}

void OverworldScene::ObjectMoveBack(Mesh* mesh) {

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

	std::stringstream ssX;
	std::stringstream ssY;
	std::stringstream ssZ;
	ssX.precision(3);
	ssX << "X:" << camera.position.x;
	ssX.precision(3);
	ssX << "Y:" << camera.position.y;
	ssZ.precision(3);
	ssZ << "Z:" << camera.position.z;

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Colors::RED, 4, 0, 10);
	modelStack.PopMatrix();
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
	camera.Init(Vector3(0, 3, 0), Vector3(5, 3, 1), Vector3(0, 1, 0), (float)100);
	currentCar = nullptr;
	currentCarObject = nullptr;

	meshList[MOON]->transform.RotateDegree(0);
	meshList[MOON]->transform.Scale(10.f);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(meshList[MOON]->transform.translate.x, meshList[MOON]->transform.translate.y, meshList[MOON]->transform.translate.z);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(0, -20, 0);

	meshList[CAR2]->transform.Translate(-22, 1.8f, 65);
	meshList[CAR2]->transform.RotateDegree(0);
	meshList[CAR2]->transform.Scale(0.15);
	meshList[CAR2]->corner[Mesh::CORNER::C1] = meshList[CAR2]->transform.translate + Vector3(1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C2] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);
	meshList[CAR2]->corner[Mesh::CORNER::C3] = meshList[CAR2]->transform.translate + Vector3(-1, 0, -1);
	meshList[CAR2]->corner[Mesh::CORNER::C4] = meshList[CAR2]->transform.translate + Vector3(1, 0, 1);
	GameObject* car = new GameObject(meshList[CAR2], meshList[CAR2]->transform);
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
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	meshList[TRUCK2]->transform.Translate(-40, 5.3f, 65);
	meshList[TRUCK2]->transform.RotateDegree(0);
	meshList[TRUCK2]->transform.Scale(0.13);
	car = new GameObject(meshList[TRUCK2], meshList[TRUCK2]->transform);
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
	sceneManager->push(car);
	car->id = sceneManager->totalObjects;

	CreateCityObjects();

	GameObject *cameraObject = new GameObject(meshList[CAMERA]);
	*cameraObject->transform = camera.position;
	sceneManager->push(cameraObject);
	cameraObject->id = sceneManager->totalObjects;
	Application::log(std::to_string(sceneManager->totalObjects));

	for (unsigned i = 0; i < NUM_TASKS; i++) {
		tasks[i] = 0;
	}
	showTaskbar = false;
	showTaskbarFrame = showTaskbarDelay =0;
	carOrigin = Vector3(-1, 0, 0);
}