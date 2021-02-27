#ifndef SCENE_OFTHEBEACH_H
#define SCENE_OFTHEBEACH_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"

class SceneOfTheBeach : public Scene 
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_OCEAN,
		GEO_ROAD,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_DRIZZLE,
		GEO_TREE,
		GEO_TREE2,
		GEO_TREE3,
		GEO_CRAB,
		GEO_DOLPHIN,
		GEO_SHIP,
		GEO_UMBRELLA,
		GEO_CABIN,
		GEO_BUOY,
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

	enum SCENE_FLOATS 
	{
		CRAB_MOVING,
		CRAB_SPEED,
		RANDOM_MOVE,
		ROTATE_DOLPHIN,

		NUM_SCENE_FLOATS
	};

	enum SCENE_BOOLS 
	{
		ROTATE,
		OPEN_TEXT_BOX,
		GL,

		NUM_SCENE_BOOLS
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
	Light light[2];
	Camera3 camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);
	void RenderQuad();
	void RenderSkybox();
	int Direction(float value);
	void RenderNPC();
	void RenderUI();
	bool isNear(Mesh* mesh, const float& distance);
public:
	SceneOfTheBeach();
	~SceneOfTheBeach();

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