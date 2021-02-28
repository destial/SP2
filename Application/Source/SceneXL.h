#ifndef SCENE_XL_H
#define SCENE_XL_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"
#include <vector>
#include "MinigameEntity.h"
#include "GameObject.h"
#include "SceneManager.h"

class SceneXL : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_LIGHTBALL,
		GEO_TEXT,
		GEO_FLOORFUTURE,
		GEO_GNOME,
		GEO_DUMMY,
		GEO_RANGE,
		GEO_ROBOT,
		GEO_BOOTH,
		GEO_WHEEL,
		GEO_BODYWHEEL,
		GEO_ROUND,
		GEO_SWING,
		GEO_TEACUP,
		GEO_TEACUPSHEL,
		GEO_JETPACK,
		GEO_MINIGUN,
		GEO_BULLET,
		GEO_SITDROPTOWER,
		GEO_DROPTOWER,
		GEO_ROAD,
		GEO_BORDERTEXT,
		GEO_UI,
		GEO_UI2,
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
		B_ROTATE_1,
		B_ROTATE_2,
		B_TALK_TO_GNOME,
		B_TALK_TO_JETPACK,
		B_TALK_TO_ROBOT,
		B_MOVING_DUMMY,
		B_GOT_GNOMED,
		B_GOT_JETPACK,
		NUM_SCENE_BOOLS,
	};

	enum SCENE_FLOATS {
		F_ROTATE_ANGLE, //rotating of my dummy target
		F_SHOOTING_SPIN,
		NUM_SCENE_FLOATS,
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
	float sceneFloats[NUM_SCENE_FLOATS];
	Mesh* meshList[NUM_GEOMETRY];
	std::vector<GameObject*> bullets;
	std::vector<MinigameEntity*> targetList;

	Light light[2];

	Camera3 camera;
	MinigameEntity* temp;
	Mesh* MinigunHold;
	Vector3 GunOrigin;	
	int tempspin;
	int dummycounter;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);
	void DetectGnome();
	void RenderGnome();
	void DetectRobot();
	void RenderRobot();
	void RenderMinigame();
	void RenderSurroundings();
	void RenderUI();
	void DetectJetpack();
	void RenderJetpack();
	void RenderSkybox();
	bool isNear(Mesh* mesh, const float& distance);
	void Minigun();
public:
	SceneXL();
	~SceneXL();

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