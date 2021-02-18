#include "SceneShaqler.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneShaqler::SceneShaqler() {}

SceneShaqler::~SceneShaqler() {}

void SceneShaqler::Init()
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
	camera.Init(Vector3(5, 4, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);

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
	light[0].position.Set(4, 20, -34);
	light[0].color.Set(1, 1, 1);
	light[0].power = 3;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(-53, 20, -31.6);
	light[1].color.Set(1, 1, 1);
	light[1].power = 3;
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

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color.tga");

	meshList[GEO_QUAD1] = MeshBuilder::GenerateQuad("quad1", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD1]->textureID = LoadTGA("Image//FutureFloor1.tga");

	meshList[GEO_QUAD2] = MeshBuilder::GenerateQuad("quad2", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD2]->textureID = LoadTGA("Image//BlackWallpaper1.tga");

	meshList[GEO_QUAD3] = MeshBuilder::GenerateQuad("quad3", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD3]->textureID = LoadTGA("Image//BlackWallpaper.tga");

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 30, 30, 1);
	meshList[GEO_SPHERE]->textureID = LoadTGA("Image//BlackWallpaper.tga");

	meshList[GEO_DESKCORNER] = MeshBuilder::GenerateOBJMTL("DeskCounter", "OBJ//deskCorner.obj", "OBJ//deskCorner.mtl");

	meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJMTL("Chair", "OBJ//chair.obj", "OBJ//chair.mtl");

	meshList[GEO_COMPUTERSCREEN] = MeshBuilder::GenerateOBJMTL("Screen", "OBJ//computerScreen.obj", "OBJ//computerScreen.mtl");

	meshList[GEO_KEYBOARD] = MeshBuilder::GenerateOBJMTL("Keyboard", "OBJ//computerKeyboard.obj", "OBJ//computerKeyboard.mtl");

	meshList[GEO_STATUE] = MeshBuilder::GenerateOBJMTL("Statue", "OBJ//tom1.obj", "OBJ//tom.mtl");

	meshList[GEO_KURENAI] = MeshBuilder::GenerateOBJMTL("Kurenai", "OBJ//Kurenai.obj", "OBJ//Kurenai.mtl"); // Try 1 first
	// ELSE TRY OBJ

	meshList[GEO_BOOKSTACK] = MeshBuilder::GenerateOBJ("Bookstack", "OBJ//BookStack.obj"); // Try 1 first
	meshList[GEO_BOOKSTACK]->textureID = LoadTGA("Image//BookStack.tga");

	//meshList[GEO_TRASHCAN] = MeshBuilder::GenerateOBJ("Bookstack", "OBJ//woodenTrashcan.obj"); // Try 1 first
	//meshList[GEO_TRASHCAN]->textureID = LoadTGA("Image//brownColour.tga");

	meshList[GEO_BOOK] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK]->textureID = LoadTGA("Image//1984book.tga");

	meshList[GEO_BOOKCASE] = MeshBuilder::GenerateOBJ("Bookcase", "OBJ//BookCase.obj"); // Try 1 first
	meshList[GEO_BOOKCASE]->textureID = LoadTGA("Image//brownColour.tga");

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

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", BLUE, 1.5, 0.8);
	meshList[GEO_UI]->textureID = LoadTGA("Image//UIBorder3.2.tga");

	bookX = -17;
	bookY = 2.85;
	bookZ = 1.6;
	rotateBook = 270; // just commit
	bookCollected = false;
	TextX = 10; // just comment
	TextY = 5; // almost done with scene 2
	TextZ = -8000;
	ScreenX = 8000;
	ScreenY = 30;

	ScreenTextX1 = 1000;
	ScreenTextX2 = 1000;
	ScreenTextY1 = 3.5;
	ScreenTextY2 = 1.2;
	ScreenTextZ1;
	ScreenTextZ2;
	Bookhasbeenbaught = false;

	x = 1;
}

void SceneShaqler::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneShaqler::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneShaqler::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneShaqler::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

void SceneShaqler::Update(double dt, Mouse mouse) {
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

	rotateBook += (float)(40 * dt);

	if (bookY > 2.85 && heightlimit == false)
	{
		bookY -= (float)(2 * dt);
	}
	else if (bookY >= -3.5 && bookY <= 3.5)
	{
		heightlimit = true;
	}
	if (bookY >= -3.5 && bookY < 3.5 && heightlimit == true)
	{
		bookY += (float)(2 * dt);
	}
	else if (bookY >= 3.5)
	{
		heightlimit = false;
	}

	if (Application::IsKeyPressed('E')) // -0.685 and 4.75 for z x// -12.5 -15
	{
		if (camera.position.x >= -15 && camera.position.x <= -12 && camera.position.z >= -0.685 && camera.position.z <= 4.75)
		{
			bookCollected = true;
			bookX = 10.9;
			bookY = 6;
			bookZ = -11.5;
		}

		if (camera.position.x >= 6 && camera.position.x <= 19.6 && camera.position.z >= -19.1 && camera.position.z <= -7.75)
		{
			bookCollected = false;
			Purchasebook = true;
			/*bookX = 10.9;
			bookY = 4;
			bookZ = 11.5;*/
		}
	}

	if (bookCollected == true)
	{
		Book();
	}

	if (Purchasebook == true)
	{
		rotateBook = 270;
		bookY = 3.1;
		TextZ = -13.5;
	}

	if (Application::IsKeyPressed('T'))
	{
		if (camera.position.x >= 6 && camera.position.x <= 19.6 && camera.position.z >= -19.1 && camera.position.z <= -7.75 && Purchasebook == true)
		{
			ScreenX = 40;
			TextX = 1000;
			bookX = 1000;
			ScreenTextX1 = 4.5;
			ScreenTextX2 = 4.5;
			Bookhasbeenbaught = true;
		}

		if (Bookhasbeenbaught == true)
		{
			ScreenTextY1 = 1000;
			ScreenTextY2 = 1000;
		 
		}
	}

	// later do purchasebook with ui

	camera.Update(dt, mouse);
	/*Application::sceneswitch = Application::SCENESHAQ;*/
}

void SceneShaqler::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneShaqler::RenderSkybox() {
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

void SceneShaqler::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.022, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_QUAD1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 15, 0);
	modelStack.Rotate(90, 0, 0, 1);
	//modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 20, 20);
	RenderMesh(meshList[GEO_QUAD2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, 15, 0);
	modelStack.Rotate(90, 0, 0, 1);
	//modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 20, 20);
	RenderMesh(meshList[GEO_QUAD2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 15, 20);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 20, 20);
	RenderMesh(meshList[GEO_QUAD2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 15, -20);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 20, 20);
	RenderMesh(meshList[GEO_QUAD2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_QUAD3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(19.8, 0, -19.8);
	modelStack.Scale(12, 8, 12);
	RenderMesh(meshList[GEO_DESKCORNER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(16, 0, -17.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(11, 9, 13);
	RenderMesh(meshList[GEO_CHAIR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10, 3, -14);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(9, 9, 9);
	RenderMesh(meshList[GEO_COMPUTERSCREEN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(11, 3.1, -14.5); //3.1
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(7, 7, 7);
	RenderMesh(meshList[GEO_KEYBOARD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(18, 0, 18);
	modelStack.Rotate(220, 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_STATUE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, 3.1, -10);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderMesh(meshList[GEO_BOOKSTACK], true);
	modelStack.PopMatrix();

	if (bookCollected == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bookX, bookY, bookZ); // 2.85
		modelStack.Rotate(rotateBook, 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_BOOK], true);
		modelStack.PopMatrix();
	}

	if (bookCollected == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_BOOK]->transform.translate.x, meshList[GEO_BOOK]->transform.translate.y, meshList[GEO_BOOK]->transform.translate.z);
		modelStack.Rotate(meshList[GEO_BOOK]->transform.rotate, 0, 1, 0);
		modelStack.Rotate(270, 0, 1, 0);
		RenderMesh(meshList[GEO_BOOK], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-17, 0, 17);
	modelStack.Scale(0.15, 0.15, 0.2);
	RenderMesh(meshList[GEO_BOOKCASE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-17, 0, -10);
	modelStack.Scale(0.15, 0.15, 0.2);
	RenderMesh(meshList[GEO_BOOKCASE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-17, 0, 3.5);
	modelStack.Scale(0.15, 0.15, 0.2);
	RenderMesh(meshList[GEO_BOOKCASE], true);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_TRASHCAN], true);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(14.2, 0, -15.8);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_KURENAI], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(TextX, 5, TextZ);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.25, 0.25, 0.25);
	RenderText(meshList[GEO_TEXT], " Press T to purchase", WHITE);
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
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), RED, 20, 0, 10);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], ".", WHITE, 200, 0, 500);
	RenderMeshOnScreen(meshList[GEO_UI], 55, ScreenX, -5); // 40 x
	RenderTextOnScreen(meshList[GEO_TEXT], "Would you like to purchase this book", BLACK, 23, ScreenTextX1, 3.5);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press T to Purchase", BLACK, 23, ScreenTextX2, 1.2);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press T to Purchase", BLACK, 23, ScreenTextX2, 1.2);

	/*RenderTextOnScreen(meshList[GEO_TEXT], ".", BLACK, 30, 0, 10);*/

}

void SceneShaqler::Book()
{
	BookHold = meshList[GEO_BOOK];
	BookHold->prevTransform = BookHold->transform;
	BookHold->transform.translate.x = camera.position.x;
	BookHold->transform.translate.z = camera.position.z;
	BookHold->transform.translate.y = camera.position.y - 1;
	Vector3 origin = (BookHold->transform.translate + BookOrigin).Normalized();
	BookHold->transform.rotate = camera.getRotation(BookOrigin);
	switch (GEO_BOOK) {
	default:
		BookOrigin = Vector3(-1, 0, 0);
		break;
	}
}

void SceneShaqler::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}