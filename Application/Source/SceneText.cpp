#include "SceneText.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneText c;
SceneText c2;

void SceneText::RenderSkybox()
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

void SceneText::RenderTree()
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



void SceneText::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

SceneText::SceneText()
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
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
	camera4.Init(Vector3(-1, 6, -1), Vector3(0, 5, 0), Vector3(0, 1, 0));

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
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	//Replace previous code
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(4, 20, -34);
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
	light[1].position.Set(-53, 20, -31.6);
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

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube",5,5,5);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0, 0, 0), 30);

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0.8, 0.6), 12, 12, 1);
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("sphere2", Color(1, 1, 1), 12, 12, 1);
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("sphere3", Color(0, 0, 0), 12, 12, 1);
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("sphere4", Color(0, 0.5, 1), 12, 12, 1);
	meshList[GEO_SPHERE5] = MeshBuilder::GenerateSphere("sphere5", Color(0, 0, 1), 12, 12, 1);


	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(0, 0, 0), 12, 12, 1);

	meshList[GEO_QUADWHITE] = MeshBuilder::GenerateQuad("quadwhite", 1, 1);


	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0.647, 0.165, 0.165), 12, 12, 1);
	meshList[GEO_HEMISPHERE2] = MeshBuilder::GenerateHemisphere("hemisphere2", Color(0.9, 0.7, 0.5), 12, 12, 1);
	meshList[GEO_HEMISPHERE3] = MeshBuilder::GenerateHemisphere("hemisphere3", Color(0.4, 0.4, 1), 12, 12, 1);
	meshList[GEO_HEMISPHERECIRCLE] = MeshBuilder::GeneratehemiSphereWithCircle("hemispherewithcircle", Color(0.118, 0.565, 1), 3);

	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(1, 0, 0), 20, 35, 20);

	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("torus", 30, 30, 30, 10, Color(0.545, 0.271, 0.075));

	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 0.8, 0.6), 50, 10, 50); //skin color
	meshList[GEO_CYLINDER2] = MeshBuilder::GenerateCylinder("cylinder2", Color(0.871, 0.722, 0.529), 50, 10, 50); //wood color

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 12, 12, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("LightBall2", Color(1, 0, 0), 36, 12, 1);

	meshList[GEO_QUADWHITE] = MeshBuilder::GenerateQuad("quadwhite", 1, 1);

	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(0.3, 0.5, 1), 1, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0.3, 0.5, 1), 12, 12, 11);
	meshList[GEO_PYRAMID] = MeshBuilder::GeneratePyramid("pyramid", Color(0.545, 0.271, 0.075), 2, 5);

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

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1",
		"OBJ//chair.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");

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

	meshList[GEO_CARRED] = MeshBuilder::GenerateOBJMTL("red colored car",
		"OBJ//carRed.obj", "OBJ//carRed.mtl");

	meshList[GEO_RACE] = MeshBuilder::GenerateOBJMTL("race car",
		"OBJ//raceFuture.obj", "OBJ//raceFuture.mtl");

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

void SceneText::Update(double dt)
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

	if (Application::IsKeyPressed('F') && camera4.position.x > -0.6 && camera4.position.x < -0.23 && camera4.position.z > -24 && camera4.position.z < -16 && justpress == false)
	{
		justpress = true;
		Application::sceneswitch = SCENEOUTSIDE;
		std::cout << "F is pressed" << std::endl;
	}
	else if (!Application::IsKeyPressed('F') && camera4.position.x > -0.6 && camera4.position.x < -0.23 && camera4.position.z > -24 && camera4.position.z < -16 && justpress == true)
	{
		justpress = false;
	}


	if (camera4.position.x > -0.6 && camera4.position.x < -0.23 && camera4.position.z > -24 && camera4.position.z < -16)
	{
		Door = true;
	}
	else
	{
		Door = false; //for door text
	}

	if (camera4.position.x > -3.46 && camera4.position.x < 18.7 && camera4.position.z > -43.3 && camera4.position.z < -26.9)
	{
		talktovill = true;
	}
	else
	{
		talktovill = false;
	}

	if (camera4.position.x > -67.1 && camera4.position.x < -50.5 && camera4.position.z > -39.7 && camera4.position.z < -18.5)
	{
		talktotom = true;
	}
	else
	{
		talktotom = false;
	}

	if (camera4.position.x > -67.1 && camera4.position.x < -54.4 && camera4.position.z > -18.3 && camera4.position.z < 7.39)
	{
		Door2 = true;
	}
	else
	{
		Door2 = false;
	}

	if (camera4.position.x > -9.17 && camera4.position.x < 19.9 && camera4.position.z > -86.1 && camera4.position.z < -54.4)
	{
		talktree1 = true;
	}
	else
	{
		talktree1 = false;
	}

	if (camera4.position.x > -5.75 && camera4.position.x < 25.2 && camera4.position.z > 28.1 && camera4.position.z < 69.7)
	{
		talktree2 = true;
	}
	else
	{
		talktree2 = false;
	}



	float carspeed = (float)(50 * dt);
	float car2speed = (float)(51 * dt);

	if (CarMoving < randomflying - carspeed * 1.5 || CarMoving > randomflying + carspeed * 1.5)
	{
		float direction = DirectionalReturn(randomflying - CarMoving);
		CarMoving += carspeed * direction;
	}
	else
	{
		randomflying = Math::RandFloatMinMax(-200, 100);
	}

	if (Car2Moving < randomflying - car2speed * 1.6 || Car2Moving > randomflying + car2speed * 1.5)
	{
		float direction = DirectionalReturn(randomflying - Car2Moving);
		Car2Moving += car2speed * direction;
	}
	else
	{
		randomflying = Math::RandFloatMinMax(-100, 200);
	}
}

void SceneText::Render()
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
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
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

	modelStack.PushMatrix();
	modelStack.Translate(7.93, 1, -35.3);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(0.45, 0.45, 0.45);
	RenderVillager();
	modelStack.PopMatrix(); // Villager from assignment 1

	modelStack.PushMatrix();
	RenderTree(); 
	modelStack.PopMatrix(); //tree

	modelStack.PushMatrix();
	modelStack.Translate(3, 0, -120); 
	RenderTree();
	modelStack.PopMatrix(); //tree

	modelStack.PushMatrix();
	modelStack.Translate(15, 11.7, 0); //y = 6
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(15, 15, 15); //8
	RenderMesh(meshList[GEO_HOUSE1], false);
	modelStack.PopMatrix(); //holograhpic house 1

	modelStack.PushMatrix();
	modelStack.Translate(-85, 8.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_HOUSE2], false);
	modelStack.PopMatrix(); //holograhpic house 2

	modelStack.PushMatrix();
	modelStack.Translate(-61.3,0,-31.3);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderMesh(meshList[GEO_TOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-22, 0, 67 + CarMoving);
	//modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10,10,10);
	RenderMesh(meshList[GEO_CARRED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-37.6, 0, -90.2 + Car2Moving);
	//modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_RACE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(7.93, 11, -38.3);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "Villager", Color(1.000, 0.843, 0.000));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(-61.3, 11, -28.3);
	modelStack.Rotate(90, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "Tom Nook", Color(1.000, 0.843, 0.000));
	modelStack.PopMatrix();

	if (Door == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press F to enter.", Color(0, 1, 0), 4, 2, 5.5f);
	}
	
	if (talktovill == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Hey! I heard you are new here.", Color(1.000, 0.843, 0.000), 2.5, 1, 5.5f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Welcome to the village!", Color(1.000, 0.843, 0.000), 3, 2, 3.0f);
	}

	if (talktotom == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "GET OUT OF MY LAWN!", Color(1.000, 0.843, 0.000), 3, 3.5, 4.5f);
		RenderTextOnScreen(meshList[GEO_TEXT], "I DON'T WANT YOU NEAR ME!", Color(1.000, 0.843, 0.000), 3, 1, 3.0f);
	}

	if (Door2 == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "It seems like its locked.", Color(1.000, 0.843, 0.000), 3, 1.5, 4.5f);
	}

	if ((talktree1 == true) or (talktree2 == true))
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "It is just a tree.", Color(1.000, 0.843, 0.000), 3, 1.5, 4.5f);
		RenderTextOnScreen(meshList[GEO_TEXT], "What did you expect?", Color(1.000, 0.843, 0.000), 3, 1, 3.0f);
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

void SceneText::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneText::RenderVillager()
{
	modelStack.PushMatrix();
	RenderVillagerHead();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderVillagerBody();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderVillagerPants();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Rotate(0.5 * Llegmove, 1, 0, 0);
	RenderVillagerLeftLeg();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Rotate(0.5 * Rlegmove, 1, 0, 0);
	RenderVillagerRightLeg();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.5, 10, 0);
	//modelStack.Rotate(LArmmove, 0, 0, 1);
	RenderVillagerLeftArm();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.5, 10, 0);
	//modelStack.Rotate(RArmmove, 0, 0, 1);
	RenderVillagerRightArm();
	modelStack.PopMatrix();
}

void SceneText::RenderVillagerHead()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 15, 0);
	modelStack.Scale(4.5, 4.0, 4.5);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix(); //HEAD

	modelStack.PushMatrix();
	modelStack.Translate(0, 16, 0);
	modelStack.Scale(0.4, 0.4, 0.4);
	RenderMesh(meshList[GEO_HEMISPHERE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1.8, 15, 3.2);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1, 1.1, 1);
	RenderMesh(meshList[GEO_SPHERE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.8, 15, 3.2);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1, 1.1, 1);
	RenderMesh(meshList[GEO_SPHERE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.65, 15.2, 3.8);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.6, 0.7, 0.6);
	RenderMesh(meshList[GEO_SPHERE3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.2, 15.2, 3.8);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.6, 0.7, 0.6);
	RenderMesh(meshList[GEO_SPHERE3], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(0, 14, 3.15);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 1);
	modelStack.Scale(0.05, 0.05, 0.05);
	RenderMesh(meshList[GEO_TORUS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3.4, 14.8, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(1.6, 2, 1.9);
	RenderMesh(meshList[GEO_HEMISPHERE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3.4, 14.8, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(1.6, 2, 1.9);
	RenderMesh(meshList[GEO_HEMISPHERE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 14, 4.2);
	modelStack.Rotate(45, 1, 0, 0);
	modelStack.Scale(0.4, 0.2, 0.2);
	RenderMesh(meshList[GEO_PYRAMID], true);
	modelStack.PopMatrix(); //nose
}

void SceneText::RenderVillagerBody()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_CONE], true);
	modelStack.PopMatrix();
}

void SceneText::RenderVillagerLeftLeg()
{
	modelStack.PushMatrix();
	modelStack.Translate(-2, 1, 0);
	modelStack.Scale(0.1, 0.08, 0.1);
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix(); //left leg

	modelStack.PushMatrix();
	modelStack.Translate(-2, -2, 0.4);
	modelStack.Scale(1.5, 1.5, 2.5);
	RenderMesh(meshList[GEO_HEMISPHERE3], true);
	modelStack.PopMatrix(); //left shoe
}

void SceneText::RenderVillagerRightLeg()
{

	modelStack.PushMatrix();
	modelStack.Translate(2, 1, 0);
	modelStack.Scale(0.1, 0.08, 0.1);
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix(); //right leg

	modelStack.PushMatrix();
	modelStack.Translate(2, -2, 0.4);
	modelStack.Scale(1.5, 1.5, 2.5);
	RenderMesh(meshList[GEO_HEMISPHERE3], true);
	modelStack.PopMatrix(); //right shoe
}

void SceneText::RenderVillagerLeftArm()
{
	modelStack.PushMatrix();

	modelStack.Translate(2.5, 0, 0);

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix(); //Left arm from villager's pov


	modelStack.PushMatrix();

	modelStack.Translate(3, 0, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix(); //Left hand from villager's pov

	modelStack.PopMatrix();
}

void SceneText::RenderVillagerRightArm()
{
	modelStack.PushMatrix();

	modelStack.Translate(-2.5, 0, 0);

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_CYLINDER], true);
	modelStack.PopMatrix(); //Right arm from villager's pov

	modelStack.PushMatrix();
	modelStack.Translate(-3, 0, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix(); //Right hand from villager's pov

	modelStack.PopMatrix();
}

void SceneText::RenderVillagerPants()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 3.5, 0);
	modelStack.Scale(1.6, 0.6, 1);
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix(); //pants
}

void SceneText::bruhmoment(int a, int b, int c)
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

int SceneText::DirectionalReturn(int value)
{

	if (value > 0)
	{
		return 1;
	}
	else if (value < 0)
	{
		return -1;
	} return -1;
}
