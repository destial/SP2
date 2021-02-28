#ifndef SCENE_SHAQEEL_H
#define SCENE_SHAQEEL_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"

class DistrictScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_QUAD1,
		GEO_QUAD2,
		GEO_CUBE,
		GEO_SPHERE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_TRUCK,
		GEO_TRASHCAN,
		GEO_STREETLIGHT,
		GEO_YELLOWSIGN,
		GEO_BUS,
		GEO_CAR1,
		GEO_CAR2,
		GEO_PLANE,
		GEO_TUNNEL,
		GEO_TREE,
		GEO_BUSH,
		GEO_BENCH,
		GEO_MART,
		GEO_ROBOBODY,
		GEO_ROBOLEFTARM,
		GEO_ROBORIGHTARM,
		GEO_ROBOLEFTLEG,
		GEO_ROBORIGHTLEG,
		GEO_DOOR,
		GEO_CITY1,
		GEO_CITY2,
		GEO_CITY3,
		GEO_UI,
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

	enum SCENE_FLOATS {

		// floats for movements
		F_TRANSLATE_WORD_Y,
		F_TRANSLATE_TRUCK_Z,
		F_TRANSLATE_BUS_Z,
		F_TRANSLATE_CAR1_Z,
		F_TRANSLATE_CAR2_Z,
		F_TRANSLATE_PLANE_X,
		F_TRANSLATE_PLANE_Z,
		F_ROTATE_DOOR,
		F_BUS_Z,
		F_ROTATE_LEFT_LEG,
		F_ROTATE_RIGHT_LEG,
		F_TRANSLATE_SPHERE_Z,
		F_TRANSLATE_SPHERE_Z_2,
		NUM_SCENE_FLOATS
	};

	enum SCENE_BOOLS {
		// bools to start and end movements
		B_ROTATE,
		B_DOOR_OPENED,
		B_STOP_OPEN_DOOR,
		B_LEFT_LEG_LIMIT,
		B_LEFT_LEG_LIMIT_2,
		B_RIGHT_LEG_LIMIT,
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
	int GLcounter;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);
	void RenderSkybox();
	bool isNear(Mesh* mesh, const float& distance);

	// render objects and background in functions below
	void RenderQuad(); 
	void Rendervehicles();
	void Rendertrees();
	void Rendercityobjects();
	void RenderNPC();
	void RenderMytext();
	void RenderUI();
public:

	DistrictScene();
	~DistrictScene();

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