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

void SceneXL::RenderMeshOnScreen(Mesh* mesh, float size, float x, float y) {
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

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0.486, 0.988, 0), 1);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(0, 0, 0), 12, 12, 1);

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

	meshList[GEO_ROAD] = MeshBuilder::GenerateQuad("road",
		Color(1, 1, 1), 50.1f);
	meshList[GEO_ROAD]->textureID = LoadTGA("Image//grass.tga");

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
	meshList[GEO_ROBOT]->transform.Translate(-19.3, 0, -40.6); //robot

	meshList[GEO_BOOTH] = MeshBuilder::GenerateOBJMTL("ticket booth",
		"OBJ//ticketboothlmao.obj", "OBJ//ticketboothlmao.mtl"); //ticket booth

	meshList[GEO_SWING] = MeshBuilder::GenerateOBJMTL("swing",
		"OBJ//swingride.obj", "OBJ//swingride.mtl"); //swing ride

	meshList[GEO_TEACUP] = MeshBuilder::GenerateOBJMTL("tea cup",
		"OBJ//teacuplmao.obj", "OBJ//teacuplmao.mtl"); //tea cup

	meshList[GEO_TEACUPSHEL] = MeshBuilder::GenerateOBJMTL("shelter tea cup",
		"OBJ//cupshelter.obj", "OBJ//cupshelter.mtl"); //shelter for the tea cup

	meshList[GEO_WHEEL] = MeshBuilder::GenerateOBJMTL("ferris wheel the wheel",
		"OBJ//bruhwheelsonly.obj", "OBJ//bruhwheelsonly.mtl");

	meshList[GEO_BODYWHEEL] = MeshBuilder::GenerateOBJMTL("ferris wheel the body",
		"OBJ//bodywheel.obj", "OBJ//bodywheel.mtl");

	meshList[GEO_SITDROPTOWER] = MeshBuilder::GenerateOBJMTL("sits for the drop tower",
		"OBJ//sitdroptower.obj", "OBJ//sitdroptower.mtl");

	meshList[GEO_DROPTOWER] = MeshBuilder::GenerateOBJMTL("drop tower",
		"OBJ//droptower.obj", "OBJ//droptower.mtl");
		
	meshList[GEO_JETPACK] = MeshBuilder::GenerateOBJMTL("jetpack",
		"OBJ//jetpack.obj", "OBJ//jetpack.mtl");
	meshList[GEO_JETPACK]->transform.Translate(-2.83, 0, 45);
	meshList[GEO_JETPACK]->transform.Scale(0.4, 0.4, 0.4);

	meshList[GEO_BORDERTEXT] = MeshBuilder::GenerateFaceQuad("border for text", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_BORDERTEXT]->textureID = LoadTGA("Image//bordertext.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.f, 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_UI2] = MeshBuilder::GenerateFaceQuad("UIBackboard", Colors::WHITE, 1.5f, 0.3f);
	meshList[GEO_UI2]->textureID = LoadTGA("Image//blueblacktextbox.tga");

	meshList[GEO_MINIGUN] = MeshBuilder::GenerateOBJMTL("Minigun", "OBJ//Minigun.obj", "OBJ//Minigun.mtl");

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("Bullet", Color(1, 1, 1), 36, 36, 1);

	Reset();

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

void SceneXL::DetectGnome() //detects if you have interacted with the gnome or not, if yes then gnomed
{
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (meshList[GEO_GNOME] && !sceneBools[B_GOT_GNOMED])
	{
		if (isNear(meshList[GEO_GNOME], (float)10.f) && sceneBools[B_TALK_TO_GNOME] == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Press F to talk.", Color(1,1,1), 4, 3*w/800, 6*h/600);

			if (Application::IsKeyPressedOnce('F'))
			{
				camera.Init(Vector3(5, 3, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);
				sceneBools[B_TALK_TO_GNOME] = true;
			}
		}
		if (isNear(meshList[GEO_GNOME], (float)10.f) && sceneBools[B_TALK_TO_GNOME] == true)
		{
			sceneBools[B_GOT_GNOMED] = true;
		}
		if (sceneBools[B_GOT_GNOMED])
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Get GNOMED! ", Colors::RED, 3, 5 * w / 800, 1 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	}
}

void SceneXL::RenderGnome() //rendering of the gnome
{
	if (meshList[GEO_GNOME] && !sceneBools[B_GOT_GNOMED])
	{
		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_GNOME]->transform.translate.x, meshList[GEO_GNOME]->transform.translate.y, meshList[GEO_GNOME]->transform.translate.z);
		modelStack.Scale(meshList[GEO_GNOME]->transform.scale.x, meshList[GEO_GNOME]->transform.scale.y, meshList[GEO_GNOME]->transform.scale.z);
		RenderMesh(meshList[GEO_GNOME], true);
		modelStack.PopMatrix(); //gnome
	}
}

void SceneXL::RenderMinigame() //rendering of the minigames
{
	for (int i = 0; i < targetList.size(); i++ && sceneBools[B_MOVING_DUMMY] == false)
	{
		if (targetList[i]) {
			modelStack.PushMatrix();
			modelStack.Translate(1.55 + targetList[i]->pos.x, 5, -79 + targetList[i]->pos.z);
			modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_DUMMY], true);
			modelStack.PopMatrix();
		}
		sceneBools[B_MOVING_DUMMY] = true;
	} //10 target dummies for the minigame/target shooting
}

void SceneXL::RenderSurroundings() //scene decorations
{
	modelStack.PushMatrix();
	modelStack.Translate(-1.07, 1, -50);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(13, 10, 13);
	RenderMesh(meshList[GEO_RANGE], true);
	modelStack.PopMatrix(); //shooting range

	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_BOOTH], true);
	modelStack.PopMatrix(); //ticket booth

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, 50);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SWING], true);
	modelStack.PopMatrix(); //swing ride

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, -30);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
	modelStack.Translate(50, 0, 0);
	RenderMesh(meshList[GEO_TEACUP], true);
	modelStack.PopMatrix(); //tea cup

	modelStack.PushMatrix();
	modelStack.Translate(-100, 0, -30);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 1, 0);
	modelStack.Translate(180, 0, 0);
	RenderMesh(meshList[GEO_TEACUP], true);
	modelStack.PopMatrix(); //tea cup 2

	modelStack.PushMatrix();
	modelStack.Translate(-110, 0, -28.3);
	//modelStack.Rotate(RotateAngle, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_TEACUPSHEL], true);
	modelStack.PopMatrix(); //tea cup shelter

	modelStack.PushMatrix();
	modelStack.Translate(100, 48, 30);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(sceneFloats[F_ROTATE_ANGLE], 0, 0, 1);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WHEEL], true);
	modelStack.PopMatrix(); //ferris wheel 

	modelStack.PushMatrix();
	modelStack.Translate(100, 10, 30);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_BODYWHEEL], true);
	modelStack.PopMatrix(); //ferris wheel body

	modelStack.PushMatrix();
	modelStack.Translate(13, 10, 100);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_SITDROPTOWER], true);
	modelStack.PopMatrix(); //the sit for the drop tower

	modelStack.PushMatrix();
	modelStack.Translate(13, 10, 100);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_DROPTOWER], true);
	modelStack.PopMatrix(); //drop tower
}

void SceneXL::DetectJetpack() //Detecting whether the jetpack has been interacted or not
{
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	if (meshList[GEO_JETPACK] && !Player::getJetpack())
	{
		if (isNear(meshList[GEO_JETPACK], (float)15.f) && Player::getJetpack() == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Press F to equip.", Color(0.541, 0.169, 0.886), 4, 1.8, 6.2);
			if (Application::IsKeyPressedOnce('F'))
			{
				sceneBools[B_TALK_TO_JETPACK] = true;
				Player::setJetpack(true);
			}
		}
		if (Player::getJetpack())
		{
			if (meshList[GEO_JETPACK])
			{
				delete meshList[GEO_JETPACK];
				meshList[GEO_JETPACK] = nullptr;
			}
		}
	}
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "JETPACK EQUIPPED", Colors::BLUE, 3, 5 * w / 800, 2 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	}
}

void SceneXL::RenderJetpack() //render my jetpack
{
	if (meshList[GEO_JETPACK] && !Player::getJetpack())
	{
		modelStack.PushMatrix();
		modelStack.Translate(meshList[GEO_JETPACK]->transform.translate.x, meshList[GEO_JETPACK]->transform.translate.y, meshList[GEO_JETPACK]->transform.translate.z);
		modelStack.Scale(meshList[GEO_JETPACK]->transform.scale.x, meshList[GEO_JETPACK]->transform.scale.y, meshList[GEO_JETPACK]->transform.scale.z);
		RenderMesh(meshList[GEO_JETPACK], true);
		modelStack.PopMatrix();
	}
}

//checks if the bullets are near the dummy for the collision
bool isNearDummy(GameObject* o1, MinigameEntity* o2, const float& distance = 1.f) {
	if (!o1 && !o2) return false;
	float d = Math::sqrt(Math::Square(o1->transform->translate.x - (1.55 + o2->pos.x)) + Math::Square(o1->transform->translate.z - (o2->pos.z - 79)));
	return (d <= distance);
}

void SceneXL::Update(double dt, Mouse mouse) {
	if (Application::previousscene != Application::SCENEXL) {
		InitGL();
	}
	sceneFloats[F_ROTATE_ANGLE] += (float)(50 * dt);

	if (Player::getJetpack() == false) //if there is no jetpack, the camera is normal
	{
		camera.Update(dt, mouse);
	}

	if (Player::getJetpack() == true)//if there is no jetpack, the camera is updated to jetpack mode, able to fly.
	{
		camera.UpdateFlying(dt, mouse);
	}

	if (sceneBools[B_MOVING_DUMMY])
	{
		srand((unsigned)time(0));
		for (int i = 0; i < targetList.size(); i++)
		{
			if (targetList[i]) {
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
		}
	}

	if (sceneBools[B_ROTATE_2])
	{
		sceneFloats[F_ROTATE_ANGLE] += (float)(100 * dt);
		if (sceneFloats[F_ROTATE_ANGLE] > 360)
			sceneBools[B_ROTATE_1] -= 360;
	}

	if (Application::IsMousePressed(0))
	{
		//Spinning animation
		if (sceneFloats[F_ROTATE_ANGLE] > 0 && tempspin == 0)
		{
			sceneFloats[F_SHOOTING_SPIN] += sceneFloats[F_SHOOTING_SPIN];
			if (sceneFloats[F_SHOOTING_SPIN] > 360)
			{
				tempspin++;
			}
		}
		else if (sceneFloats[F_SHOOTING_SPIN] > 0 && tempspin == 1)
		{
			sceneFloats[F_SHOOTING_SPIN] -= sceneFloats[F_SHOOTING_SPIN];
			if (bullets.size() < Player::getAmmo())
			{
				//Uses player view and gives it to the bullet so that it will travel forward and not follow player view after he turns
				GameObject* bullet = new GameObject(meshList[GEO_BULLET]);
				bullet->transform->Translate(camera.position.x, camera.position.y - 2, camera.position.z);
				bullet->transform->Scale(0.5);
				Vector3 view = (camera.target - camera.position).Normalized();
				Vector3 right = view.Cross(camera.up).Normalized();
				right.y = 0;
				Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
				bullet->view = face;
				bullets.push_back(bullet);
				Player::setAmmo(Player::getAmmo() - 1); //Bullet - 1
			}
			else {
				for (auto b : bullets) {
					if (b) {
						delete b;
					}
				}
				bullets.clear();
				Player::setAmmo(256);//Set ammo back to 256
			}
		}
		else
		{
			sceneFloats[F_SHOOTING_SPIN] += 0.01;
			tempspin = 0;
		}
	}

	for (auto bullet : bullets) {
		bullet->transform->translate += (bullet->view)*3;
	}

	for (std::vector<GameObject*>::iterator b = bullets.begin(); b != bullets.end(); b++) {
		bool hit = false;
		if ((*b)->transform->translate.x > camera.bounds || (*b)->transform->translate.x < -camera.bounds) {
			bullets.erase(b);
			break;
		}
		if ((*b)->transform->translate.z > camera.bounds || (*b)->transform->translate.z < -camera.bounds - 50) {
			bullets.erase(b);
			break;
		}
		for (std::vector<MinigameEntity*>::iterator d = targetList.begin(); d != targetList.end(); d++) {
			if (isNearDummy(*b, *d, 1.f)) {
				bullets.erase(b);
				targetList.erase(d);
				hit = true;
				break;
			}
		}
		if (hit) break;
	}
	
	Minigun();
	if (camera.position.x <= 50 &&
		camera.position.x >= 36 &&
		camera.position.z <= -20 &&
		camera.position.z >= -40)
	{
		camera.position.x = 10;
		camera.position.z = 10;
		Application::sceneswitch = Application::OVERWORLD;
		Application::previousscene = Application::SCENEXL;
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

void SceneXL::InitGLXray()
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

void SceneXL::Update(double dt)
{
	Mouse mouse;
	Update(dt, mouse);
}

void SceneXL::DetectRobot()
{
	if (isNear(meshList[GEO_ROBOT], (float)5.f) && sceneBools[B_TALK_TO_ROBOT] == false)
	{
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		//RenderMeshOnScreen(meshList[GEO_BORDERTEXT], 75, 45, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start", Color(0.541, 0.169, 0.886), 4, 1.8 * w / 800, 6.2 * h / 600);
		RenderTextOnScreen(meshList[GEO_TEXT], "Your simulation.", Color(0.541, 0.169, 0.886), 4, 1.8 * w / 800, 4.2 * h / 600);
		if (Application::IsKeyPressedOnce('F'))
		{
			camera.Init(Vector3(1.1, 10, -30.3), Vector3(1.1, 10, -30.4), Vector3(0, 1, 0), (float)50);
			sceneBools[B_TALK_TO_ROBOT] = true;
		}
	}
	if (sceneBools[B_TALK_TO_ROBOT] == true)
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

	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_MINIGUN]->transform.translate.x, meshList[GEO_MINIGUN]->transform.translate.y, meshList[GEO_MINIGUN]->transform.translate.z);
	modelStack.Rotate(meshList[GEO_MINIGUN]->transform.rotate, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(0.07, 0.07, 0.07);
	modelStack.Rotate(sceneFloats[F_SHOOTING_SPIN], 0, 1, 0);
	RenderMesh(meshList[GEO_MINIGUN], true);
	modelStack.PopMatrix();

	RenderSurroundings();

	DetectRobot();
	RenderRobot();
	RenderMeshOnScreen(meshList[GEO_UI2], 40, 40, 7);
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
	RenderJetpack();
	DetectJetpack();



	DetectGnome();
	RenderGnome();
	for (auto o : bullets) {
		if (o && o->transform) {
			modelStack.PushMatrix();
			modelStack.Translate(o->transform->translate.x, o->transform->translate.y, o->transform->translate.z);
			modelStack.Rotate(o->transform->rotate, 0, 1, 0);
			modelStack.Scale(o->transform->scale.x, o->transform->scale.y, o->transform->scale.z);
			RenderMesh(o->mesh, true);
			modelStack.PopMatrix();
		}
	}
	RenderUI();
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);

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
	RenderTextOnScreen(meshList[GEO_TEXT], ssX.str() + ssY.str() + ssZ.str(), Color(0.863, 0.078, 0.235), 4, 0, 10);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(50, 20,-38.5);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderText(meshList[GEO_TEXT], "This way to the Beachshelt!", Colors::BLACK);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, 0.1, -30.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.3, 3, 1.6);
	RenderMesh(meshList[GEO_ROAD], false);
	modelStack.PopMatrix(); //road
}

void SceneXL::RenderUI() {
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	RenderMeshOnScreen(meshList[GEO_UI], 25, 12.5, 53.75 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(Player::getHealth()), Colors::BLACK, 2, 0.5 * w / 800, 19 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo:" + std::to_string(Player::getAmmo()), Colors::BLACK, 2, 0.5 * w / 800, 18 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], "Money:" + std::to_string(Player::getMoney()), Colors::BLACK, 2, 0.5 * w / 800, 17 * h / 600);
	RenderTextOnScreen(meshList[GEO_TEXT], ".", Colors::WHITE, 0, 0, 0);
}

void SceneXL::RenderRobot()
{
	modelStack.PushMatrix();
	modelStack.Translate(meshList[GEO_ROBOT]->transform.translate.x, meshList[GEO_ROBOT]->transform.translate.y, meshList[GEO_ROBOT]->transform.translate.z);
	modelStack.Scale(14, 14, 14);
	RenderMesh(meshList[GEO_ROBOT], true);
	modelStack.PopMatrix(); // robot
}

void SceneXL::Minigun()
{
	//Sets the gun to follow the player position and rotation
	GunOrigin = Vector3(-1, 0, 0);
	MinigunHold = meshList[GEO_MINIGUN];
	MinigunHold->prevTransform = MinigunHold->transform;
	MinigunHold->transform.translate.x = camera.position.x;
	MinigunHold->transform.translate.z = camera.position.z;
	MinigunHold->transform.translate.y = camera.position.y - 2;
	Vector3 origin = (MinigunHold->transform.translate + GunOrigin).Normalized();
	MinigunHold->transform.rotate = camera.getRotation(GunOrigin);
}

void SceneXL::Exit() {
	for (auto mesh : meshList) {
		if (mesh) delete mesh;
	}
	for (auto b : bullets) {
		if (b) {
			delete b;
		}
	}
	bullets.clear();
	for (auto d : targetList) {
		if (d) {
			delete d;
		}
	}
	targetList.clear();
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneXL::Reset() {
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	camera.Init(Vector3(5, 10, 5), Vector3(1, 0.5, 1), Vector3(0, 1, 0), (float)50);
	camera.orthographic_size = 45.f;

	for (auto b : bullets) {
		if (b) {
			delete b;
		}
	}
	bullets.clear();
	for (auto d : targetList) {
		if (d) {
			delete d;
		}
	}
	targetList.clear();
	sceneFloats[F_SHOOTING_SPIN] = 0.01;
	sceneBools[B_TALK_TO_GNOME] = false;
	sceneBools[B_TALK_TO_ROBOT] = false;
	sceneBools[B_TALK_TO_JETPACK] = false;
	sceneBools[B_GOT_GNOMED] = false;
	sceneBools[B_MOVING_DUMMY] = false;
	srand((unsigned)time(0));

	for (int i = 0; i < 10; i++)
	{
		int x = (rand() % 3);

		int y = (rand() % 3);

		int z = (rand() % 3);
		temp = new MinigameEntity;
		temp->pos = Vector3(x, y, z);

		targetList.push_back(temp);
	}
}