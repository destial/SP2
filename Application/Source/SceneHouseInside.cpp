#include "SceneHouseInside.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneHouseInside c;
SceneHouseInside c2;


void SceneHouseInside::RenderSkybox()
{
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

void SceneHouseInside::RenderTree()
{
	modelStack.PushMatrix();
	modelStack.Translate(3, 0, 50);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_MODEL7], false);
	modelStack.PopMatrix(); //tree

	modelStack.PushMatrix();
	modelStack.Translate(3, 0, 50);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_MODEL8], false);
	modelStack.PopMatrix(); //tree
}



void SceneHouseInside::RenderMesh(Mesh* mesh, bool enableLight)
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
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SceneHouseInside::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneHouseInside::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

SceneHouseInside::SceneHouseInside()
{
}

SceneHouseInside::~SceneHouseInside()
{
}

void SceneHouseInside::Init()
{
	
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 0;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //bg colour

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	// Enable blending
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
	camera4.Init(Vector3(8.72, 6, -16.4), Vector3(0, 5, 0), Vector3(0, 1, 0));
	camera4.yaw = 45;

	//shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	//m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//Shading.fragmentshader");
	//m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader");
	//m_programID = LoadShaders("Shader//Texture.vertexshader",
	//	"Shader//Texture.fragmentshader");
	//m_programID = LoadShaders("Shader//Texture.vertexshader",
	//	"Shader//Blending.fragmentshader");

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
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//Replace previous code
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
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


	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1,1,0), 1);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 1), 12);

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 1), 12, 12, 1);
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("sphere2", Color(1, 0, 0), 12, 12, 1);
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("sphere3", Color(0, 1, 0), 12, 12, 1);
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("sphere4", Color(0, 0.5, 1), 12, 12, 1);
	meshList[GEO_SPHERE5] = MeshBuilder::GenerateSphere("sphere5", Color(0, 0, 1), 12, 12, 1);
	meshList[GEO_SPHERE6] = MeshBuilder::GenerateSphere("sphere6", Color(0.5, 1, 0), 12, 12, 1);
	meshList[GEO_SPHERE7] = MeshBuilder::GenerateSphere("sphere7", Color(1, 0.5, 0.5), 12, 12, 1);
	meshList[GEO_SPHERE8] = MeshBuilder::GenerateSphere("sphere8", Color(1, 0.5, 1), 12, 12, 1);
	meshList[GEO_SPHERE9] = MeshBuilder::GenerateSphere("sphere9", Color(0, 1, 0.7), 12, 12, 1);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 12, 12, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("LightBall2", Color(1, 0, 0), 36, 12, 1);

	meshList[GEO_QUADWHITE] = MeshBuilder::GenerateQuad("quadwhite", 1, 1);

	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(0.3, 0.5, 1), 1, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0.3, 0.5, 1), 12, 12, 11);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color.tga");
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

	meshList[GEO_SIDM] = MeshBuilder::GenerateQuad("sidm",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_SIDM]->textureID = LoadTGA("Image//sidm.tga");

	//meshList[GEO_TWOMAD] = MeshBuilder::GenerateQuad("twomad",
	//	Color(1, 1, 1), 50.1f);
	//meshList[GEO_TWOMAD]->textureID = LoadTGA("Image//twomad.tga");


	meshList[GEO_TWOMAD] = MeshBuilder::GenerateQuad("floor",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_TWOMAD]->textureID = LoadTGA("Image//grass.tga");


	meshList[GEO_JORDAN] = MeshBuilder::GenerateQuad("jordan",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_JORDAN]->textureID = LoadTGA("Image//jordanface.tga");

	//meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1",
	//	"OBJ//chair.obj");
	//meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");

	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJ("model2",
		"OBJ//doorman.obj");
	meshList[GEO_MODEL2]->textureID = LoadTGA("Image//doorman.tga");

	meshList[GEO_MODEL3] = MeshBuilder::GenerateOBJ("model3",
		"OBJ//dartboard.obj");
	meshList[GEO_MODEL3]->textureID = LoadTGA("Image//dartboard.tga");

	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJ("model4",
		"OBJ//dart.obj");
	meshList[GEO_MODEL4]->textureID = LoadTGA("Image//dart.tga");

	meshList[GEO_MODEL6] = MeshBuilder::GenerateOBJ("model6",
		"OBJ//shoe.obj");
	meshList[GEO_MODEL6]->textureID = LoadTGA("Image//shoe.tga");

	meshList[GEO_MODEL7] = MeshBuilder::GenerateOBJ("model7",
		"OBJ//MapleTreeStem.obj");
	meshList[GEO_MODEL7]->textureID = LoadTGA("Image//maplebark.tga");

	meshList[GEO_MODEL8] = MeshBuilder::GenerateOBJ("model7",
		"OBJ//MapleTreeLeaves.obj");
	meshList[GEO_MODEL8]->textureID = LoadTGA("Image//mapleleaf.tga");

	meshList[GEO_HOUSE1] = MeshBuilder::GenerateOBJ("house1",
		"OBJ//house2.obj");
	meshList[GEO_HOUSE1]->textureID = LoadTGA("Image//house1.tga"); //holographic house 1

	meshList[GEO_HOUSE2] = MeshBuilder::GenerateOBJ("house2",
		"OBJ//house.obj");
	meshList[GEO_HOUSE2]->textureID = LoadTGA("Image//house1.tga"); //holographic house 2

	meshList[GEO_TOM] = MeshBuilder::GenerateOBJMTL("tomnook",
		"OBJ//tom1.obj", "OBJ//tom.mtl");

	meshList[GEO_BED] = MeshBuilder::GenerateOBJMTL("bed",
		"OBJ//bed.obj", "OBJ//bed.mtl");

	meshList[GEO_DESK] = MeshBuilder::GenerateOBJMTL("desk",
		"OBJ//desk.obj", "OBJ//desk.mtl");

	meshList[GEO_DESK] = MeshBuilder::GenerateOBJMTL("desk",
		"OBJ//desk.obj", "OBJ//desk.mtl");

	meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJMTL("chair",
		"OBJ//chair.obj", "OBJ//chair.mtl");

	meshList[GEO_KEYBOARD] = MeshBuilder::GenerateOBJMTL("keyboard",
		"OBJ//computerKeyboard.obj", "OBJ//computerKeyboard.mtl");

	meshList[GEO_MOUSE] = MeshBuilder::GenerateOBJMTL("mouse",
		"OBJ//computerMouse.obj", "OBJ//computerMouse.mtl");

	meshList[GEO_SCREEN] = MeshBuilder::GenerateOBJMTL("screen",
		"OBJ//computerScreen.obj", "OBJ//computerScreen.mtl");

	meshList[GEO_TV] = MeshBuilder::GenerateOBJMTL("tv",
		"OBJ//televisionModern.obj", "OBJ//televisionModern.mtl");

	meshList[GEO_SOFA] = MeshBuilder::GenerateOBJMTL("sofa",
		"OBJ//Sofa.obj", "OBJ//Sofa.mtl");

	meshList[GEO_CABINET] = MeshBuilder::GenerateOBJMTL("cabinet for tv",
		"OBJ//TvCabinet.obj", "OBJ//TvCabinet.mtl");

	meshList[GEO_BEDCABINET] = MeshBuilder::GenerateOBJMTL("bed cabinet",
		"OBJ//cabinetBed.obj", "OBJ//cabinetBed.mtl");

	//meshList[GEO_CAFE] = MeshBuilder::GenerateOBJMTL("cafe",
	//	"OBJ//sobj_cafe.obj", "OBJ//sobj_cafe.mtl");

	meshList[GEO_HOUSETYPE1] = MeshBuilder::GenerateOBJMTL("housetype1",
		"OBJ//house_type01.obj", "OBJ//house_type01.mtl");

	//meshList[GEO_MAILBOX] = MeshBuilder::GenerateOBJMTL("mailbox",
	//	"OBJ//int_post_A00.obj", "OBJ//int_post_A00.mtl");

	meshList[GEO_COTTAGE] = MeshBuilder::GenerateOBJMTL("cottage",
		"OBJ//cottage_obj.obj", "OBJ//cottage_obj.mtl"); //cottage_diffuse
	meshList[GEO_COTTAGE]->textureID =
		LoadTGA("Image//cottage_diffuse.tga");


	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
}

void SceneHouseInside::Update(double dt)
{
	
	rotateAngle += (float)(20 * dt);
	rotateAngle2 += (float)(50 * dt);
	translateX += (float)(5 * dt);
	scaleAll += (float)(1 * dt);
	LSPEED = 10.f;
	camera4.Update(dt);

	fps = 1.f / dt;

	if (rotate == true)
	{
		rotateAngle += (float)(100 * dt);
		if (rotateAngle > 360)
			rotate -= 360;

		rotateAngle2 += (float)(100 * dt);
		if (rotateAngle2 > 360)
			rotate -= 360;
	}


	if (Application::IsKeyPressed(0x31))
		glEnable(GL_CULL_FACE);

	else if (Application::IsKeyPressed(0x32))
		glDisable(GL_CULL_FACE);

	else if (Application::IsKeyPressed(0x33))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode

	else if (Application::IsKeyPressed(0x34))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

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

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		light[1].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		light[1].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);

	}
	else if (Application::IsKeyPressed('7'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		light[1].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	}

	if (Application::IsKeyPressed('F') && camera4.position.x > -0.0238 && camera4.position.x < 37.7 && camera4.position.z > -28.6 && camera4.position.z < 23.8 && justpress == false)
	{
		justpress = true;
		Application::sceneswitch = SCENEHOUSEINSIDE;
		std::cout << "F is pressed" << std::endl;
	}
	else if(!Application::IsKeyPressed('F') && camera4.position.x > -0.0238 && camera4.position.x < 37.7 && camera4.position.z > -28.6 && camera4.position.z < 23.8 && justpress == true)
	{
		justpress = false;
	}

	if (camera4.position.x > -0.0238 && camera4.position.x < 37.7 && camera4.position.z > -28.6 && camera4.position.z < 23.8)
	{
		DoorExit = true;
	}
	else
	{
		DoorExit = false;
	}
} 

void SceneHouseInside::Render()
{
	//Clear the color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	viewStack.LoadIdentity();
	viewStack.LookAt(camera4.position.x, camera4.position.y, camera4.position.z,
		camera4.target.x, camera4.target.y, camera4.target.z,
		camera4.up.x, camera4.up.y, camera4.up.z
	);
	modelStack.LoadIdentity();

	Mtx44 lmao;
	lmao.SetToPerspective(camera4.orthographic_size, 800.f / 600.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(lmao);

	RenderMesh(meshList[GEO_AXES], false);

	//Pass light position to shader
	//Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	//glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	//cube
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 1, 0);
	//RenderMesh(meshList[GEO_SPHERE], true);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_LIGHTBALL2], false);
	//modelStack.PopMatrix();


	//modelStack.PushMatrix();
	//modelStack.Translate(2.5, 1, 0);
	//RenderMesh(meshList[GEO_SPHERE2], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(5, 1, 0);
	//RenderMesh(meshList[GEO_SPHERE3], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 1, 2.5);
	//RenderMesh(meshList[GEO_SPHERE4], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(2.5, 1, 2.5);
	//RenderMesh(meshList[GEO_SPHERE5], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(5, 1, 2.5);
	//RenderMesh(meshList[GEO_SPHERE6], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(2.5, 1, -2.5);
	//RenderMesh(meshList[GEO_SPHERE7], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 1, -2.5);
	//RenderMesh(meshList[GEO_SPHERE8], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(5, 1, -2.5);
	//RenderMesh(meshList[GEO_SPHERE9], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Rotate(-90, 1, 0, 0);
	//
	//RenderMesh(meshList[GEO_QUAD], false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera4.position.x, camera4.position.y, camera4.position.z);
	modelStack.Scale(4.5,4.5,4.5);
	RenderSkybox();
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_SIDM], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(-30, 0.15, 0);
	////modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(0.5, 0.5, 5.5);
	//RenderMesh(meshList[GEO_TWOMAD], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(5, 15, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(0.05, 0.05, 0.05);
	//RenderMesh(meshList[GEO_JORDAN], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(-5, 15, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(0.05, 0.05, 0.05);
	//RenderMesh(meshList[GEO_JORDAN], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 15, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(0.05, 0.05, 0.05);
	//RenderMesh(meshList[GEO_JORDAN], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//scale, translate, rotate
	//RenderMesh(meshList[GEO_MODEL1], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(5, 0, 0);
	//RenderMesh(meshList[GEO_MODEL2], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//RenderMesh(meshList[GEO_MODEL3], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//RenderMesh(meshList[GEO_MODEL4], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//RenderMesh(meshList[GEO_MODEL5], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(10, 0, 0);
	//RenderMesh(meshList[GEO_MODEL6], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderTree(); 
	//modelStack.PopMatrix(); //tree

	//modelStack.PushMatrix();
	//modelStack.Translate(3, 0, -120); 
	//RenderTree();
	//modelStack.PopMatrix(); //tree

	modelStack.PushMatrix();
	modelStack.Translate(15, 11.7, 0); //y = 6
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(15, 15, 15); //8
	RenderMesh(meshList[GEO_HOUSE1], false);
	modelStack.PopMatrix(); //holograhpic house 1

	//modelStack.PushMatrix();
	//modelStack.Translate(-85, 8.5, 0);
	//modelStack.Rotate(90, 0, 1, 0);
	//modelStack.Scale(15, 15, 15);
	//RenderMesh(meshList[GEO_HOUSE2], false);
	//modelStack.PopMatrix(); //holograhpic house 2

	//modelStack.PushMatrix();
	//////modelStack.Translate(-3, 0, 0);
	//modelStack.Scale(0.1, 0.1, 0.1);
	//RenderMesh(meshList[GEO_ISABELLE], false);
	//modelStack.PopMatrix(); //isabelle



	//modelStack.PushMatrix();
	//modelStack.Translate(5, 0, 0);
	//modelStack.Scale(0.1, 0.1, 0.1);
	//RenderMesh(meshList[GEO_TOM], true);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(30.1,0,6.26);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_BED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(13.1,0,8.63);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_DESK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(8.9,0, 4.67);
	modelStack.Rotate(135, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_CHAIR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10.84, 3.8, 6.52);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_KEYBOARD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(7.88, 3.8, 4.1);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_MOUSE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(9.98, 3.8, 8.4);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SCREEN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(22.8,0,5);
	//modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SOFA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15.8, 0, -20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_CABINET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(19.8,3.2,-21);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_TV], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(19.8, 3.2, -21);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_TV], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(31.1, 0 ,-7.65);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BEDCABINET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(31.1, 4.5, -7.65);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BEDCABINET], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 5);
	//modelStack.Scale(100, 100, 100);
	//RenderMesh(meshList[GEO_HOUSETYPE1], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////modelStack.Scale(100, 100, 100);
	//RenderMesh(meshList[GEO_COTTAGE], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//RenderText(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0));
	//modelStack.PopMatrix();

	if (DoorExit == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press F to exit.", Color(0, 1, 0), 4, 2, 5.5f);
	}

	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS:" + std::to_string(fps), Color(0, 1, 0), 4, 0, 1);
	modelStack.PopMatrix();


	std::stringstream ssX;
	std::stringstream ssY;
	std::stringstream ssZ;
	ssX.precision(3);
	ssX << "X:" << camera4.position.x;
	ssX.precision(3);
	ssX << "Y:" << camera4.position.y;
	ssZ.precision(3);
	ssZ << "Z:" << camera4.position.z;

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Color(0.863, 0.078, 0.235), 4, 0, 10);
	modelStack.PopMatrix();
}

void SceneHouseInside::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneHouseInside::bruhmoment(int a, int b, int c)
{
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();


	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(a, 0, 0, 1);
	translate.SetToTranslation(b, c, 1);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
}
