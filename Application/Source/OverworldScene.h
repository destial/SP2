#ifndef OVERWORLD_SCENE_H
#define OVERWORLD_SCENE_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "SceneManager.h"
#include "Light.h"
#include "Camera3.h"

class OverworldScene : public Scene {
	enum GEOMETRY_TYPE {
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_SPHERE,
		GEO_GROUND,
		GEO_TEXT,
		MOON,
		SIDEBAR,
		GEO_UI,

		STREETLIGHT,
		NUM_ROADOBJECTS,

		SKYSCRAPER1,
		SKYSCRAPER2,
		NY_BUILDING,
		NUM_BUILDINGS,

		TRUCK1,
		TRUCK2,
		CAR1,
		CAR2,
		BUS1,
		COP,
		PLANE,
		NUM_CAR,

		GEO_ROBOBODY,
		GEO_ROBORIGHTLEG,
		GEO_ROBOLEFTLEG,
		GEO_ROBORIGHTARM,
		GEO_ROBOLEFTARM,

		CAMERA,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1NABLED,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	enum TASKS {
		STEAL_CAR,
		ENTER_BUILDING,
		SPRINT,
		WALK,
		LOOK,
		JUMP,

		NUM_TASKS
	};
private:
	MS modelStack, viewStack, projectionStack;
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];
	Mesh* currentCar;
	GameObject* currentCarObject;
	Vector3 carOrigin;
	Light light[2];
	Camera3 camera;
	SceneManager* sceneManager;

	bool tasks[NUM_TASKS];
	bool showTaskbar;
	bool leftleglimit;
	int showTaskbarFrame;
	int showTaskbarDelay;
	float rotateleftleg;
	float translateSphereZ;
	float translateSphereZ2;
	float translateSphereZ3;
	float translateSphereZ4;
	float translateSphereX1;
	float translateSphereX2;
	int GLcounter;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);
	void RenderSkybox();
	void RenderObjects();
	void RenderTasks();
	void RenderRobo();
	void RenderSpeedometer();
	void RoadTeleport();
	void RenderTeleportText();
	void CompleteTasks();
	void GetInCar();
	void EnterBuilding();
	void DetectCollision();
	void UpdateRobo(double& dt);
	void UpdatePlanes(double& dt);
	void CreateCityObjects();
	void MoveBack();
	void RenderUI();
	bool isNear(Mesh* mesh, const float& distance = 1.f);
	bool isNearObject(GameObject* o, const float& distance = 1.f);
	bool isHit(GameObject* o1, GameObject* o2, const float& distance = 1.f);

public:
	OverworldScene();
	~OverworldScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Update(double dt, Mouse mouse);
	virtual void InitGL();
	virtual void InitGLXray();
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
};

#endif