#ifndef SCENE_HOUSEINSIDE_H
#define SCENE_HOUSEINSIDE_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera.h"
#include "Camera2.h"
#include "CameraSceneInsideHouse.h"




class SceneHouseInside : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_SPHERE6,
		GEO_SPHERE7,
		GEO_SPHERE8,
		GEO_SPHERE9,
		GEO_HEMISPHERE,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL2,
		GEO_QUADWHITE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_SIDM,
		GEO_TWOMAD,
		GEO_JORDAN,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_MODEL7,
		GEO_MODEL8,
		GEO_ISABELLE,
		GEO_TOM,
		GEO_DOOR,
		GEO_HOUSE1,
		GEO_HOUSE2,
		GEO_HOUSETYPE1,
		GEO_COTTAGE,
		GEO_MAILBOX,
		GEO_CAFE,
		//add these enum in GEOMETRY_TYPE before NUM_GEOMETRY
		GEO_TEXT,
		GEO_BED,
		GEO_DESK,
		GEO_CHAIR,
		GEO_KEYBOARD,
		GEO_SCREEN,
		GEO_MOUSE,
		GEO_TV,
		GEO_SOFA,
		GEO_CABINET,
		GEO_BEDCABINET,
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


private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	void RenderSkybox();
	void RenderTree();

	float rotateAngle;
	float rotateAngle2;
	float translateX;
	float scaleAll;
	float LSPEED;
	float fps;

	bool rotate;
	Mesh* meshList[NUM_GEOMETRY];

	Light light[2];

	CameraSceneInsideHouse camera4;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

public:

	SceneHouseInside();
	~SceneHouseInside();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool DoorExit;
	bool justpress = false;

	void bruhmoment(int a, int b, int c);

	MS modelStack, viewStack, projectionStack;
};

#endif