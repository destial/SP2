#include "SceneRyan.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneRyan::SceneRyan() {}

SceneRyan::~SceneRyan() {}

void SceneRyan::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //bg colour

		//shaders
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
	m_parameters[U_MATERIAL_ALPHA] = glGetUniformLocation(m_programID, "material.kAlpha");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
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

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(5, 10, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);

	// Make sure you pass uniform parameters after glUseProgram()

	//Replace previous code
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 0, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 0, 0);
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

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",Color(1, 1, 1), 10.1f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Ocean.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front",Color(1, 1, 1), 50.1f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//OceanBack.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back",Color(1, 1, 1), 50.1f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//OceanFront.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top",Color(1, 1, 1), 50.1f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//OceanTop.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom",Color(1, 1, 1), 50.1f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//OceanBottom.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left",Color(1, 1, 1), 50.1f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//OceanLeft.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right",Color(1, 1, 1), 50.1f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//OceanRight.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_SHARKTOP] = MeshBuilder::GenerateOBJMTL("SharkTop","OBJ//SharkTop.obj", "OBJ//SharkTop.mtl");

	meshList[GEO_SHARKBTM] = MeshBuilder::GenerateOBJMTL("SharkBtm", "OBJ//SharkBtm.obj", "OBJ//SharkBtm.mtl");

	meshList[GEO_MINIGUN] = MeshBuilder::GenerateOBJMTL("Minigun", "OBJ//Minigun.obj", "OBJ//Minigun.mtl");

	meshList[GEO_BEACH] = MeshBuilder::GenerateHemisphere("Beach", Color(1, 1, 1), 36, 36, 1);
	meshList[GEO_BEACH]->material.kAmbient.Set(0.900, 0.843, 0.000);
	meshList[GEO_BEACH]->material.kDiffuse.Set(0.900, 0.843, 0.000);
	meshList[GEO_BEACH]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BEACH]->material.kShininess = 0.6f;

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Bullet", Color(1, 1, 1), 36, 36, 1);

	rotate = true;
	sharkattack = false;
	Tempcounter = 0;
	rotatetail = 0;
	sharkcircle = 1;
	camera.SharkPos.x = 100;
	camera.SharkPos.y = 0;
	camera.SharkPos.z = 0;

	Application::log("Scene Ryan initialized");
}

void SceneRyan::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneRyan::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneRyan::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10); //size of screen UI
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

void SceneRyan::RenderMeshOnScreen(Mesh* mesh, Color color, float size, float x, float y) {
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

void SceneRyan::Update(double dt, Mouse mouse) {
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

	if (rotate == true)
	{
		rotatetail += 1;
		if (rotatetail > 20)
		{
			rotate = false;
		}
	}
	else if (rotate == false)
	{
		rotatetail -= 1;
		if (rotatetail < -20)
		{
			rotate = true;
		}
	}
	if ((sharkcircle % 720) == 0 && sharkattack == false)
	{
		sharkattack = true;
	}
	if (sharkattack == true && (sharkcircle % 720) == 0)
	{
		if (Tempcounter == 0)
		{
			camera.SharkChaseinit();
			Vector3 origin = Vector3(-1, 0, 0);
			sharkdir = camera.getSharkRotation(origin) - 90;
			Tempcounter = 1;
		}
		
		std::cout << camera.SharkPos.x << std::endl;
		if (camera.SharkPos.x > 30)
		{
			rotateshark -= 0.5;
			camera.SharkPos.y += 0.1;
			camera.SharkChaseMove();
		}
		else if (camera.SharkPos.x > 30)
		{
			rotateshark += 0.5;
			camera.SharkPos.y -= 0.1;
			camera.SharkChaseMove();
		}
		else if (camera.SharkPos.x > -100)
		{
			rotateshark += 0.5;
			camera.SharkPos.y -= 0.1;
			camera.SharkChaseMove();
		}
		else
		{
			camera.SharkPos.x = 100;
			camera.SharkPos.z = 0;
			sharkdir = 0;
			sharkattack = false;
			Tempcounter = 0;
			sharkcircle = 1;
			rotateshark = 0;
			camera.SharkPos.y = 0;
		}
	}
	else
	{
		sharkcircle += 1;
		sharkcircleangle += 0.5;
		temptime = dt;
	}
	

	if (Application::IsKeyPressed('F')) 
	{

		Minigun();
		Shootingspin += 3;
	}



	camera.Update(dt, mouse);
}

void SceneRyan::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneRyan::RenderSkybox() {
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
	modelStack.Rotate(-90, 0, 1, 0);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
}

void SceneRyan::Render()
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
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Scale(4.5, 4.5, 4.5);
	RenderSkybox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-sharkcircleangle, 0, 1, 0);
	modelStack.Translate(camera.SharkPos.x, camera.SharkPos.y, camera.SharkPos.z);
	modelStack.Rotate(sharkdir, 0, 1, 0);
	modelStack.Rotate(rotateshark, 1, 0, 0);
	modelStack.Scale(3, 3, 3);
	RenderShark();
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, -145, 0);
	modelStack.Scale(200, 150, 200);
	RenderMesh(meshList[GEO_BEACH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_MINIGUN]->transform.translate.x, meshList[GEO_MINIGUN]->transform.translate.y, meshList[GEO_MINIGUN]->transform.translate.z);
	modelStack.Rotate(meshList[GEO_MINIGUN]->transform.rotate, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.07, 0.07, 0.07);
	modelStack.Rotate(Shootingspin, 0, 1, 0);
	RenderMesh(meshList[GEO_MINIGUN], true);
	modelStack.PopMatrix();





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
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Color(0.863, 0.078, 0.235), 20, 0, 10);
	modelStack.PopMatrix();


}
void SceneRyan::RenderShark()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -2.5, 4);
	modelStack.Scale(1.1, 1.1, 1.1);
	RenderMesh(meshList[GEO_SHARKTOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotatetail, 0, 1, 0);
	RenderMesh(meshList[GEO_SHARKBTM], true);
	modelStack.PopMatrix();
}

//NOT MY STUFF
void SceneRyan::Minigun()
{
	MinigunHold = meshList[GEO_MINIGUN];
	MinigunHold->prevTransform = MinigunHold->transform;
	MinigunHold->transform.translate.x = camera.position.x;
	MinigunHold->transform.translate.z = camera.position.z;
	MinigunHold->transform.translate.y = camera.position.y - 2;
	Vector3 origin = (MinigunHold->transform.translate + GunOrigin).Normalized();
	MinigunHold->transform.rotate = camera.getRotation(GunOrigin);
	switch (GEO_MINIGUN) {
	default:
		GunOrigin = Vector3(-1, 0, 0);
		break;
	}
}

//void SceneRyan::Bullets() {
//	for (int i = 0; i < 256; i++) {
//		if (bullet_array[i] != nullptr) {
//			modelStack.PushMatrix();
//			Vector3 pos = bullet_array[i]->getCurrPos();
//
//			modelStack.Translate(pos.x, pos.y, pos.z);
//			modelStack.Rotate(bullet_array[i]->GetDirection(), 0, 1, 0);
//			modelStack.Rotate(90, 1, 0, 0);
//			modelStack.Scale(0.05f, 0.05f, 0.05f);
//			RenderMesh(meshList[GEO_BULLET], true);
//
//			modelStack.PopMatrix();
//		}
//	}
//}

void SceneRyan::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}