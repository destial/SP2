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

void SceneShaqler::Init() {
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Generate shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 0, 0);
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 0, 0);

	InitGL();

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

	meshList[GEO_QUAD4] = MeshBuilder::GenerateQuad("quad4", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD4]->textureID = LoadTGA("Image//bigdj.tga");

	meshList[GEO_QUAD5] = MeshBuilder::GenerateQuad("quad5", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD5]->textureID = LoadTGA("Image//obamium.tga");

	meshList[GEO_QUAD6] = MeshBuilder::GenerateQuad("quad6", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD6]->textureID = LoadTGA("Image//knuckles.tga");

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 4, 4, 4);
	meshList[GEO_CUBE]->material.kAmbient.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.5f, 1.f, 0.5f);
	meshList[GEO_CUBE]->material.kShininess = 1.f;

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 30, 30, 1);
	meshList[GEO_SPHERE]->textureID = LoadTGA("Image//BlackWallpaper.tga");

	meshList[GEO_SOFA] = MeshBuilder::GenerateOBJMTL("Sofa", "OBJ//BlueSofa.obj", "OBJ//BlueSofa.mtl");

	meshList[GEO_DESKCORNER] = MeshBuilder::GenerateOBJMTL("DeskCounter", "OBJ//deskCorner.obj", "OBJ//deskCorner.mtl");

	meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJMTL("Chair", "OBJ//chair.obj", "OBJ//chair.mtl");

	meshList[GEO_COMPUTERSCREEN] = MeshBuilder::GenerateOBJMTL("Screen", "OBJ//computerScreen.obj", "OBJ//computerScreen.mtl");

	meshList[GEO_KEYBOARD] = MeshBuilder::GenerateOBJMTL("Keyboard", "OBJ//computerKeyboard.obj", "OBJ//computerKeyboard.mtl");

	meshList[GEO_STATUE] = MeshBuilder::GenerateOBJMTL("Statue", "OBJ//tom1.obj", "OBJ//tom.mtl");

	//meshList[GEO_TABLEANDCHAIR] = MeshBuilder::GenerateOBJMTL("Tableandchair", "OBJ//diningSet.obj", "OBJ//diningSet.mtl"); // Try 1 first

	meshList[GEO_KURENAI] = MeshBuilder::GenerateOBJMTL("Kurenai", "OBJ//Kurenai.obj", "OBJ//Kurenai.mtl"); // Try 1 first
	// ELSE TRY OBJ

	meshList[GEO_SAMIDAL] = MeshBuilder::GenerateOBJMTL("Samidal", "OBJ//Samidalwithoutarms.obj", "OBJ//Samidalwithoutarms.mtl");

	meshList[GEO_SAMIDALRIGHTARM] = MeshBuilder::GenerateOBJMTL("Samidal", "OBJ//Samidalrightarm.obj", "OBJ//Samidalrightarm.mtl");

	meshList[GEO_SAMIDALLEFTARM] = MeshBuilder::GenerateOBJMTL("Samidal", "OBJ//Samidalleftarm.obj", "OBJ//Samidalleftarm.mtl");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJMTL("Samidal", "OBJ//BrownRoundTable.obj", "OBJ//BrownRoundTable.mtl");

	meshList[GEO_PAINTING] = MeshBuilder::GenerateOBJ("Painting", "OBJ//paintingframe.obj"); // Try 1 first
	meshList[GEO_PAINTING]->textureID = LoadTGA("Image//brownColour.tga");

	meshList[GEO_BOOKSTACK] = MeshBuilder::GenerateOBJ("Bookstack", "OBJ//BookStack.obj"); // Try 1 first
	meshList[GEO_BOOKSTACK]->textureID = LoadTGA("Image//BookStack.tga");

	meshList[GEO_BOOK] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK]->textureID = LoadTGA("Image//1984book.tga");

	meshList[GEO_BOOK2] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK2]->textureID = LoadTGA("Image//brownColour.tga");

	meshList[GEO_BOOK3] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK3]->textureID = LoadTGA("Image//blueColour.tga");

	meshList[GEO_BOOK4] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK4]->textureID = LoadTGA("Image//redColour.tga");

	meshList[GEO_BOOK5] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK5]->textureID = LoadTGA("Image//orangeColour.tga");

	meshList[GEO_BOOK6] = MeshBuilder::GenerateOBJ("Book", "OBJ//1984book.obj"); // Try 1 first
	meshList[GEO_BOOK6]->textureID = LoadTGA("Image//greenColour.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("Book", "OBJ//MartDoor1.obj"); // Try 1 first
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//RedColour.tga");

	meshList[GEO_BOOKCASE] = MeshBuilder::GenerateOBJ("Bookcase", "OBJ//BookCase.obj"); // Try 1 first
	meshList[GEO_BOOKCASE]->textureID = LoadTGA("Image//brownColour.tga");

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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_UI2] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.5f, 0.4f);
	meshList[GEO_UI2]->textureID = LoadTGA("Image//blueblacktextbox.tga");

	Reset();

	Application::log("Scene Shaqler initialized");
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

void SceneShaqler::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

void SceneShaqler::Update(double dt, Mouse mouse) {
	if (Application::previousscene != Application::SCENESHAQLER) {
		InitGL();
	}
	sceneFloats[F_ROTATE_BOOK] += (float)(40 * dt);

	if (sceneVectors[V_BOOK].y > 2.85 && sceneBools[B_HEIGHT_LIMIT] == false)
	{
		sceneVectors[V_BOOK].y -= (float)(2 * dt);
	}
	else if (sceneVectors[V_BOOK].y >= -3.5 && sceneVectors[V_BOOK].y <= 3.5)
	{
		sceneBools[B_HEIGHT_LIMIT] = true;
	}
	if (sceneVectors[V_BOOK].y >= -3.5 && sceneVectors[V_BOOK].y < 3.5 && sceneBools[B_HEIGHT_LIMIT] == true)
	{
		sceneVectors[V_BOOK].y += (float)(2 * dt);
	}
	else if (sceneVectors[V_BOOK].y >= 3.5)
	{
		sceneBools[B_HEIGHT_LIMIT] = false;
	}


	if (Application::IsKeyPressed('E')) // E to pickup book from shelf and place on counter. Also to open door
	{
		if (camera.position.x >= -15 && camera.position.x <= -12 && camera.position.z >= -0.685 
			&& camera.position.z <= 4.75 && sceneBools[B_BOOK_BOUGHT] == false) // pickup book, bookhasbeenbaught = false stops from continuously picking up book
		{
			sceneBools[B_BOOK_COLLECTED] = true;
			sceneVectors[V_BOOK].x = 10.9;
			sceneVectors[V_BOOK].y = 6;
			sceneVectors[V_BOOK].z = -11.5;
		}

		if (camera.position.x >= 6 && camera.position.x <= 19.6 && camera.position.z >= -19.1 && camera.position.z <= -7.75) // place book on counter
		{
			sceneBools[B_BOOK_COLLECTED] = false;
			sceneBools[B_PURCHASE_BOOK] = true;
		}

		if (camera.position.x >= 1.13 && camera.position.x <= 6.6 && camera.position.z >= 14 && camera.position.z <= 20 && !sceneBools[B_STOP_OPEN_DOOR])
		{
			sceneBools[B_DOOR_OPENED] = true;
		}

		if (camera.position.x <= -13 && camera.position.x >= -20 && camera.position.z >= 4.4 && camera.position.z <= 9
			&& sceneBools[B_FINDMONEY] == false && sceneBools[B_CASHCOLLECTED] == false)
		{
			sceneBools[B_FINDMONEY] = true;
		}
	}

	if (sceneBools[B_DOOR_OPENED] == true)
	{
		if (!sceneBools[B_STOP_OPEN_DOOR])
		{
			sceneFloats[F_ROTATE_DOOR] += (float)(40 * dt);
		}

		if (sceneFloats[F_ROTATE_DOOR] >= 290)
		{
			sceneBools[B_STOP_OPEN_DOOR] = true;
		}
	}

	if (sceneBools[B_STOP_OPEN_DOOR] == true)
	{
		sceneVectors[V_TEXT_WORLD_SCREEN].y = 8.3;
	}

	if (sceneBools[B_BOOK_COLLECTED] == true)
	{
		Book(); // this is how book will teleport on to player after pressing e at the shelf
	}

	if (sceneBools[B_PURCHASE_BOOK] == true)
	{
		sceneFloats[F_ROTATE_BOOK] = 270;
		sceneVectors[V_BOOK].y = 3.1;
		sceneVectors[V_TEXT].z = -13.5;
	}

	if (Application::IsKeyPressed('T'))
	{
		if (camera.position.x >= 6 && camera.position.x <= 19.6 && camera.position.z >= -19.1 && camera.position.z <= -7.75 && sceneBools[B_BOOK_BOUGHT] == false)
		{
			sceneBools[B_IS_BUYING] = true;
			
			if (sceneBools[B_IS_BUYING] == true)
			{
				sceneVectors[V_SCREEN].x = 40;
				sceneVectors[V_TEXT].x = 1000; // world text 
			}



		}
	}

	if (Application::IsKeyPressedOnce('Y'))
	{
		if (sceneBools[B_IS_BUYING] == true)
		{
			sceneVectors[V_BOOK].x = 1000;
			sceneBools[B_IS_BUYING] = false;
			sceneBools[B_BOOK_BOUGHT] = true;
			Player::setMoney(Player::getMoney() - 30);
		}

		if (sceneBools[B_FINDMONEY] == true)
		{
			Player::setMoney(Player::getMoney() + 50);
			sceneBools[B_FINDMONEY] = false;
			sceneBools[B_CASHCOLLECTED] = true;

		}

	}

	if (Application::IsKeyPressed('N'))
	{
		sceneBools[B_IS_BUYING] = false;
	}

	if (Application::IsKeyPressedOnce('F'))
	{
		if (camera.position.x >= 1.13 && camera.position.x <= 6.6 && camera.position.z >= 14 && camera.position.z <= 20
			&& sceneBools[B_STOP_OPEN_DOOR] == true)
		{
			Application::sceneswitch = Application::SCENESHAQ;
		}	
	}

	if (Application::IsKeyPressedOnce(VK_ESCAPE)) {
		Application::sceneswitch = Application::STARTSCENE;
	}

	camera.Update(dt, mouse);
}

void SceneShaqler::InitGL()
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

void SceneShaqler::InitGLXray()
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
	view.SetToPerspective(camera.orthographic_size, Application::GetWindowWidth() / Application::GetWindowHeight(), 0.1f, 1000.f);
	projectionStack.LoadMatrix(view);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();
	RenderSkybox();

	RenderWalls();
	RenderInatimateobjects();
	RenderNPC();
	RenderBooks();
	RenderBooks2();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -27);
	RenderBooks2();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -13.5);
	RenderBooks2();
	modelStack.PopMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	RenderUI();
	RenderText();
}

void SceneShaqler::RenderUI()
{
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (sceneBools[B_IS_BUYING] == true && sceneBools[B_BOOK_BOUGHT] == false)
	{
		RenderMeshOnScreen(meshList[GEO_UI2], 50, 40, 8 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], "Would you like to purchase this book?", Colors::WHITE, 2, 5 * w / 800, 3 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], "(Y) Yes   (N) No", Colors::WHITE, 2, 10 * w / 800, 2 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	}

	if (sceneBools[B_FINDMONEY] == true && sceneBools[B_CASHCOLLECTED] == false)
	{
		RenderMeshOnScreen(meshList[GEO_UI2], 50, 40, 8 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], "You found some cash behind this painting! ", Colors::WHITE, 2, 5 * w / 800, 3 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press Y to collect", Colors::WHITE, 2, 10 * w / 800, 2 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	}

	RenderMeshOnScreen(meshList[GEO_UI], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(Player::getHealth()), Colors::BLACK, 2, 0.5 * w / 800, 19 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:" + std::to_string(Player::getAmmo()), Colors::BLACK, 2, 0.5 * w / 800, 18 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:" + std::to_string(Player::getMoney()), Colors::BLACK, 2, 0.5 * w / 800, 17 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
}

// walls rendered
void SceneShaqler::RenderWalls() 
{
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
	modelStack.PopMatrix(); // x -18.2 z 15.5

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
}

// objs like couch, shelves and table
void SceneShaqler::RenderInatimateobjects()
{
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
	modelStack.Translate(17.2, 0, -7.1);
	modelStack.Rotate(300, 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_STATUE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, 3.1, -10);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderMesh(meshList[GEO_BOOKSTACK], true);
	modelStack.PopMatrix();

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

	modelStack.PushMatrix();
	modelStack.Translate(1.3, 0, 19.55);
	modelStack.Rotate(sceneFloats[F_ROTATE_DOOR], 0, 1, 0);
	modelStack.Scale(1.12, 1.18, 1);
	RenderMesh(meshList[GEO_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(17.2, 0, 8.5);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.7, 0.7,0.7);
	RenderMesh(meshList[GEO_SOFA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(19.7, 6, 1);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(1.3, 1.3, 1.3);
	RenderMesh(meshList[GEO_PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(19.8, 8.15, 1.025);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.75, 3.5, 1.1);
	RenderMesh(meshList[GEO_QUAD4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19.7, 6, 6.2);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.3, 1.3, 1.3);
	RenderMesh(meshList[GEO_PAINTING], true); // hidden money
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19.8, 8.15, 6.25);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(1.2, 1.2, 1);
	RenderMesh(meshList[GEO_QUAD5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19.7, 6, -6.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.3, 1.3, 1.3);
	RenderMesh(meshList[GEO_PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19.8, 8.15, -6.45);
	modelStack.Rotate(270, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.75, 1, 1);
	RenderMesh(meshList[GEO_QUAD6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(16, 0, 17);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.4, 0.4, 0.4);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

}

// text
void SceneShaqler::RenderText()
{
	modelStack.PushMatrix();
	modelStack.Translate(sceneVectors[V_TEXT].x, 5, sceneVectors[V_TEXT].z);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(0.25, 0.25, 0.25);
	RenderText(meshList[GEO_TEXT], " Press T to purchase", Colors::WHITE);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5.85, sceneVectors[V_TEXT_WORLD_SCREEN].y, 19.5);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderText(meshList[GEO_TEXT], " Press F to leave", Colors::WHITE);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3.4, 0, 21.9);
	modelStack.Scale(1, 4, 1);
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::BLACK, 200, 0, 500);

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
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Color(0.863, 0.078, 0.235), 2, 0, 7);
	modelStack.PopMatrix();
}

// floating book
void SceneShaqler:: RenderBooks()
{
	if (sceneBools[B_BOOK_COLLECTED] == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(sceneVectors[V_BOOK].x, sceneVectors[V_BOOK].y, sceneVectors[V_BOOK].z); // 2.85
		modelStack.Rotate(sceneFloats[F_ROTATE_BOOK], 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_BOOK], true);
		modelStack.PopMatrix();
	}

	if (sceneBools[B_BOOK_COLLECTED] == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_BOOK]->transform.translate.x, meshList[GEO_BOOK]->transform.translate.y, meshList[GEO_BOOK]->transform.translate.z);
		modelStack.Rotate(meshList[GEO_BOOK]->transform.rotate, 0, 1, 0);
		modelStack.Rotate(270, 0, 1, 0);
		RenderMesh(meshList[GEO_BOOK], true);
		modelStack.PopMatrix();
	}
}

// regular books
void SceneShaqler::RenderBooks2()
{
	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 2.85, 11.5); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 3.1, 11.5); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 3.35, 11.5); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16, 5.5, 10.85); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(30, 0, 0, 1);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 5, 15.1); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 5.25, 15.1); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 0.5, 15.1); // 2.85
	/*modelStack.Rotate(90, 0, 1, 0);*/
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 0.85, 15.1); // 2.85
	/*modelStack.Rotate(90, 0, 1, 0);*/
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 7.35, 11.5); // 2.85
	/*modelStack.Rotate(90, 0, 1, 0);*/
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 7.70, 11.5); // 2.85
	/*modelStack.Rotate(90, 0, 1, 0);*/
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.5, 7.35, 11.5); // 2.85
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.8, 1.05, 12.45); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-40, 0, 0, 1);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-16.8, 7.95, 16); // 2.85
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-40, 0, 0, 1);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_BOOK6], true);
	modelStack.PopMatrix();
}

// function to put book at "waist" of camera. 
void SceneShaqler::Book()
{
	BookHold = meshList[GEO_BOOK];
	BookHold->prevTransform = BookHold->transform;
	BookHold->transform.translate.x = camera.position.x;
	BookHold->transform.translate.z = camera.position.z;
	BookHold->transform.translate.y = camera.position.y - 1;
	Vector3 origin = (BookHold->transform.translate + sceneVectors[V_BOOK_ORIGIN]).Normalized();
	BookHold->transform.rotate = camera.getRotation(sceneVectors[V_BOOK_ORIGIN]);
}

// cashier to purchase from and stranger in mart
void SceneShaqler::RenderNPC()
{
	modelStack.PushMatrix();
	modelStack.Translate(14.2, 0, -15.8);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(3.5, 3.5, 3.5);
	RenderMesh(meshList[GEO_KURENAI], true);
	modelStack.PopMatrix();

	// use hierach modelling

	modelStack.PushMatrix();
	modelStack.Translate(-9, 3.5, -13.8);
	modelStack.Scale(0.25, 0.25, 0.25);
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PushMatrix();
	modelStack.Translate(0.32, -13, 1);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(16, 16, 16);
	RenderMesh(meshList[GEO_SAMIDAL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.32, 6, -3);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Rotate(40, 0, 0, 1);
	modelStack.Scale(21, 18, 18);
	RenderMesh(meshList[GEO_SAMIDALRIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.32, 6, 5);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Rotate(-40, 0, 0, 1);
	modelStack.Scale(21, 18, 18);
	RenderMesh(meshList[GEO_SAMIDALLEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

}

void SceneShaqler::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneShaqler::Reset() {
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(3.87, 5, 15.7), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)19);
	camera.orthographic_size = 45.f;

	sceneVectors[V_BOOK].x = -17;
	sceneVectors[V_BOOK].y = 2.85;
	sceneVectors[V_BOOK].z = 1.6;
	sceneFloats[F_ROTATE_BOOK] = 270;
	sceneFloats[F_ROTATE_DOOR] = 180;
	sceneBools[B_BOOK_COLLECTED] = false;
	sceneVectors[V_TEXT].x = 10;
	sceneVectors[V_TEXT].y = 5;
	sceneVectors[V_TEXT].z = -8000;
	sceneVectors[V_SCREEN].x = 8000;
	sceneVectors[V_SCREEN].y = 30;
	sceneVectors[V_TEXT_WORLD_SCREEN].y = 1000;

	sceneBools[B_BOOK_BOUGHT] = false;
	sceneBools[B_FINDMONEY] = false;
	sceneBools[B_CASHCOLLECTED] = false;

	sceneFloats[F_AMOUNT] = 30;
	sceneVectors[V_BOOK_ORIGIN] = Vector3(-1, 0, 0);
}