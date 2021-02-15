#include "SceneLight.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "Utility.h"

void SceneLight::RenderMesh(Mesh* mesh, bool enableLight)
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
	mesh->Render();
}

SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

void SceneLight::Init()
{
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 0;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //bg colour

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera3.Init(Vector3(4, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0),(float) 50);

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1,1,0), 1);
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

	meshList[GEO_QUADWHITE] = MeshBuilder::GenerateQuad("quadwhite", 1, 1);

	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(0.3, 0.5, 1), 1, 1); 
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(0.3, 0.5, 1), 12, 12, 11);
	//shaders
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//Shading.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	glUseProgram(m_programID);

	light[0].position.Set(0, 5, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

}

void SceneLight::Update(double dt)
{
	rotateAngle += (float)(20 * dt);
	rotateAngle2 += (float)(50 * dt);
	translateX += (float)(5 * dt);
	scaleAll += (float)(1 * dt);
	LSPEED = 10.f;
	camera3.Update(dt);

	if (rotate == true)
	{
		rotateAngle += (float)(100 * dt);
		if (rotateAngle > 360)
			rotate -= 360;

		rotateAngle2 += (float)(100 * dt);
		if (rotateAngle2  > 360)
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
}

void SceneLight::Render()
{
	//Clear the color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera3.position.x, camera3.position.y, camera3.position.z,
		camera3.target.x, camera3.target.y, camera3.target.z,
		camera3.up.x, camera3.up.y, camera3.up.z
	);
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	//Pass light position to shader
	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	//cube
	modelStack.PushMatrix();
	modelStack.Translate(0, 1, 0);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.5, 1, 0);
	RenderMesh(meshList[GEO_SPHERE2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5, 1, 0);
	RenderMesh(meshList[GEO_SPHERE3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1, 2.5);
	RenderMesh(meshList[GEO_SPHERE4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.5, 1, 2.5);
	RenderMesh(meshList[GEO_SPHERE5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5, 1, 2.5);
	RenderMesh(meshList[GEO_SPHERE6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.5, 1, -2.5);
	RenderMesh(meshList[GEO_SPHERE7], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1, -2.5);
	RenderMesh(meshList[GEO_SPHERE8], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5, 1, -2.5);
	RenderMesh(meshList[GEO_SPHERE9], true);
	modelStack.PopMatrix();
}

void SceneLight::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneLight	::bruhmoment(int a, int b, int c)
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
