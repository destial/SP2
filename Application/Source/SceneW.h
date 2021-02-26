#ifndef SCENE_W_H
#define SCENE_W_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"
#include "SceneManager.h"

class SceneW : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_CUBE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_DOOR,
		GEO_DOOROPEN,
		GEO_WALL,
		GEO_UI,
		GEO_CLAYMORE,
		GEO_ARMOURPLATE,
		GEO_HELMET,
		MWALL,
		BOX,
		BOXOPEN,
		CHESTTOP,
		CHESTBOTTOM,
		GEO_WALLDOOR,
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

	enum SCENE_BOOLS {
		B_ROTATE,
		B_DOOR_OPENED,
		B_STOP_DOOR_OPEN,
		B_CHEST_OPEN_1,
		B_CHEST_OPEN_2,
		B_CHEST_OPEN_3,
		B_CHEST_OPEN_4,
		B_CHEST_OPEN_5,
		B_CHEST_LIMIT_1,
		B_CHEST_LIMIT_2,
		B_CHEST_LIMIT_3,
		B_CHEST_LIMIT_4,
		B_CHEST_LIMIT_5,
		B_CLAYMORE_SPAWN,
		B_ARMOR_SPAWN,
		B_HELMET_SPAWN,
		B_HEIGHT_LIMIT_1,
		B_HEIGHT_LIMIT_2,
		B_HEIGHT_LIMIT_3,
		B_HEIGHT_LIMIT_4,
		B_HEIGHT_LIMIT_5,
		B_CLAYMORE_LIMIT,
		B_HELMET_LIMIT,
		B_ARMOR_LIMIT,
		B_COLLECTED_CLAYMORE,
		B_COLLECTED_ARMOR,
		B_COLLECTED_HELMET,
		B_DOOR_OPEN,
		B_PARTICLES_SPAWN_1,
		B_PARTICLES_LIMIT_1,
		B_GL,
		NUM_SCENE_BOOLS,
	};

	enum SCENE_FLOATS {
		F_ROTATE_CHEST_1,
		F_ROTATE_CHEST_2,
		F_ROTATE_CHEST_3,
		F_ROTATE_CHEST_4,
		F_ROTATE_CHEST_5,
		F_ROTATE_DOOR,
		NUM_SCENE_FLOATS,
	};

	enum SCENE_VECTORS {
		V_ARMOR,
		V_HELMET,
		V_CLAYMORE,
		V_SCALE_CLAYMORE,
		V_SCALE_ARMOR,
		V_SCALE_HELMET,
		V_SCALE_SP,
		V_TRANSLATE_SPHERE_1,
		V_TRANSLATE_SPHERE_2,
		NUM_SCENE_VECTORS,
	};
private:
	MS modelStack, viewStack, projectionStack;
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	bool sceneBools[NUM_SCENE_BOOLS];
	Vector3 sceneVectors[NUM_SCENE_VECTORS];
	float sceneFloats[NUM_SCENE_FLOATS];

	int countChest;

	Mesh* meshList[NUM_GEOMETRY];
	SceneManager* sceneManager;

	Light light[2];

	Camera3 camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);
	void RenderMaze();
	void CreateMaze();
	void RenderSkybox();
	void RenderUI();
	void RenderRoom();
	void RenderItems();
	void RenderParticles();
	void moveBack(GameObject* object);
	void DetectCollision();
	bool isNear(GameObject* object);
	void RenderBoxes();
public:
	SceneW();
	~SceneW();

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