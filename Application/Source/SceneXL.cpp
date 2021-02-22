#include "SceneXL.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>


SceneXL::SceneXL() {}

SceneXL::~SceneXL() {}

void SceneXL::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y)
{
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

void SceneXL::Init()
{
	talktognome = false;
	talktorobot = false;
	GotGnome = false;
	// Clear background color to blue
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
	camera.Init(Vector3(5, 10, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0),(float) 50);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(0, 0, 0), 12, 12, 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color.tga");

	meshList[GEO_ROBOTT] = MeshBuilder::GenerateQuad("quad",
		Color(1, 1, 1), 10.1f);
	meshList[GEO_ROBOTT]->textureID = LoadTGA("Image//house1.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//bluecloud_bk.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//bluecloud_ft.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//bluecloud_dn.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bluecloud_up.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//bluecloud_lf.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//bluecloud_rt.tga");

	meshList[GEO_FLOORFUTURE] = MeshBuilder::GenerateQuad("floor",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_FLOORFUTURE]->textureID = LoadTGA("Image//floorfuture.tga");

	meshList[GEO_GNOME] = MeshBuilder::GenerateOBJMTL("gnome",
		"OBJ//gnomelol.obj", "OBJ//gnomelol.mtl");
	meshList[GEO_GNOME]->transform.Translate(0, 0, 0);
	meshList[GEO_GNOME]->transform.Scale(0.022);

	meshList[GEO_DUMMY] = MeshBuilder::GenerateOBJMTL("dummy target",
		"OBJ//dummy.obj", "OBJ//dummy.mtl");

	meshList[GEO_RANGE] = MeshBuilder::GenerateOBJMTL("shooting range",
		"OBJ//shootingGallery.obj", "OBJ//ShootingGallery.mtl");

	meshList[GEO_ROBOT] = MeshBuilder::GenerateOBJ("robot",
		"OBJ//robo.obj");
	meshList[GEO_ROBOT]->textureID = LoadTGA("Image//robo_normal.tga"); //robo_normal
	meshList[GEO_ROBOT]->transform.Translate(-19.3, 0, -43.6); //robot

	meshList[GEO_BOOTH] = MeshBuilder::GenerateOBJMTL("ticket booth",
		"OBJ//ticketboothlmao.obj", "OBJ//ticketboothlmao.mtl"); //ticket booth

	meshList[GEO_SWING] = MeshBuilder::GenerateOBJMTL("swing",
		"OBJ//swingride.obj", "OBJ//swingride.mtl"); //swing ride

	meshList[GEO_TEACUP] = MeshBuilder::GenerateOBJMTL("tea cup",
		"OBJ//teacuplmao.obj", "OBJ//teacuplmao.mtl"); //tea cup

	meshList[GEO_TEACUPSHEL] = MeshBuilder::GenerateOBJMTL("shelter tea cup",
		"OBJ//cupshelter.obj", "OBJ//cupshelter.mtl"); //shelter for the tea cup

	meshList[GEO_WHEEL] = MeshBuilder::GenerateOBJMTL("ferris wheel",
		"OBJ//ferriswheels.obj", "OBJ//ferriswheels.mtl"); //shelter for the tea cup

	meshList[GEO_BORDERTEXT] = MeshBuilder::GenerateFaceQuad("border for text", WHITE, 1.f, 1.f);
	meshList[GEO_BORDERTEXT]->textureID = LoadTGA("Image//bordertext.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	srand((unsigned)time(0));

	for (int i = 0; i < 10; i++) 
	{
		int x = (rand() % 5);

		int y = (rand() % 5);

		int z = (rand() % 5);
		temp = new MinigameEntity;
		temp->pos = Vector3(x, y, z);

		targetList.push_back(temp); 
	}
	Application::log("Scene XL initialized");
}

void SceneXL::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneXL::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneXL::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
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


bool SceneXL::isNear(Mesh* mesh, const float& distance)
{
	if (mesh->type == Mesh::TYPE::OBJECT) {
		return (camera.position.x <= (mesh->transform.translate.x + distance) &&
			camera.position.x >= (mesh->transform.translate.x - distance)) &&
			(camera.position.z <= (mesh->transform.translate.z + distance) &&
				camera.position.z >= (mesh->transform.translate.z - distance));
	}
	return false;
}

void SceneXL::DetectGnome()
{
	if (meshList[GEO_GNOME] && !GotGnome)
	{
		if (isNear(meshList[GEO_GNOME], (float)10.f) && talktognome == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Press F to talk.", Color(1,1,1), 4, 3, 6);

			if (Application::IsKeyPressedOnce('F'))
			{
				camera.Init(Vector3(5, 3, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);
				talktognome = true;
			}
		}
		if (isNear(meshList[GEO_GNOME], (float)10.f) && talktognome == true)
		{
			GotGnome = true;
		}
		if (GotGnome)
		{
			if (meshList[GEO_GNOME])
			{
				delete meshList[GEO_GNOME];
				meshList[GEO_GNOME] = nullptr;
				//camera.invert();
			}
		}
	}
	else  
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Get GNOMED! ", RED, 4, 0, 0);
	}
}

void SceneXL::RenderGnome()
{
	if (meshList[GEO_GNOME] && !GotGnome)
	{
		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_GNOME]->transform.translate.x, meshList[GEO_GNOME]->transform.translate.y, meshList[GEO_GNOME]->transform.translate.z);
		modelStack.Scale(meshList[GEO_GNOME]->transform.scale.x, meshList[GEO_GNOME]->transform.scale.y, meshList[GEO_GNOME]->transform.scale.z);
		RenderMesh(meshList[GEO_GNOME], true);
		modelStack.PopMatrix(); //gnome
	}
}

void SceneXL::RenderMinigame()
{
	for (int i = 0; i < targetList.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(1.55 + targetList[i]->pos.x, 5, -79 + targetList[i]->pos.z);
		modelStack.Rotate(RotateAngle, 0, 1, 0);
		modelStack.Scale(1.5,1.5,1.5);
		RenderMesh(meshList[GEO_DUMMY], true);
		modelStack.PopMatrix();
	} //10 target dummies for the minigame/target shooting
}

void SceneXL::RenderSurroundings()
{
	modelStack.PushMatrix();
	modelStack.Translate(-1.07, 1, -50);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_RANGE], true);
	modelStack.PopMatrix(); //shooting range

	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BOOTH], true);
	modelStack.PopMatrix(); //ticket booth

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 50);
	modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SWING], true);
	modelStack.PopMatrix(); //swing ride

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, -30);
	modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Translate(50, 0, 0);
	RenderMesh(meshList[GEO_TEACUP], true);
	modelStack.PopMatrix(); //tea cup

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, -30);
	modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Translate(180, 0, 0);
	RenderMesh(meshList[GEO_TEACUP], true);
	modelStack.PopMatrix(); //tea cup 2

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, -30);
	//modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_TEACUPSHEL], true);
	modelStack.PopMatrix(); //tea cup shelter

	modelStack.PushMatrix();
	modelStack.Translate(100, 0, 30);
	modelStack.Rotate(RotateAngle, 1, 0, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_WHEEL], true);
	modelStack.PopMatrix(); //tea cup shelter


}

void SceneXL::Update(double dt, Mouse mouse) {

	RotateAngle += (float)(50 * dt);

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

	camera.Update(dt, mouse);

	srand((unsigned)time(0));
	for (int i = 0; i < targetList.size(); i++)
	{
		if (targetList[i]->timemoved == 0 || targetList[i]->timemoved > 1)
		{
			targetList[i]->DirectionDummy = 1 + (rand() % 6);
			if (targetList[i]->timemoved > 1)
			{
				targetList[i]->timemoved = 0;
			}
		}
		targetList[i]->MoveDummy(dt);
	}

	if (Rotate == true)
	{
		RotateAngle += (float)(100 * dt);
		if (RotateAngle > 360)
			rotate -= 360;
	}
}

void SceneXL::InitGL()
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

void SceneXL::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneXL::DetectRobot()
{
	if (isNear(meshList[GEO_ROBOT], (float)5.f) && talktorobot == false)
	{
		//RenderMeshOnScreen(meshList[GEO_BORDERTEXT], 75, 45, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start", Color(0.541, 0.169, 0.886), 4, 1.8, 6.2);
		RenderTextOnScreen(meshList[GEO_TEXT], "Your simulation.", Color(0.541, 0.169, 0.886), 4, 1.8, 4.2);
		if (Application::IsKeyPressedOnce('F'))
		{
			camera.Init(Vector3(1.1, 10, -30.3), Vector3(1.1, 10, -30.4), Vector3(0, 1, 0), (float)50);
			talktorobot = true;
		}
	}
	if (talktorobot == true)
	{
		RenderMinigame();
	}
}

void SceneXL::RenderSkybox() {
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

void SceneXL::Render()
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
	modelStack.PopMatrix(); //axis

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Scale(4.5, 4.5, 4.5);
	RenderSkybox();
	modelStack.PopMatrix(); //skybox

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_FLOORFUTURE], false);
	modelStack.PopMatrix(); //floor 

	RenderSurroundings();

	DetectRobot();
	RenderRobot();

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
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Color(0.000, 1.000, 0.498), 4, 0, 10);
	modelStack.PopMatrix();

	DetectGnome();
	RenderGnome();
}

void SceneXL::RenderRobot()
{
	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_ROBOT]->transform.translate.x, meshList[GEO_ROBOT]->transform.translate.y, meshList[GEO_ROBOT]->transform.translate.z);
	modelStack.Scale(14, 14, 14);
	RenderMesh(meshList[GEO_ROBOT], true);
	modelStack.PopMatrix(); // robot
}

void SceneXL::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}