#ifndef SCENE_RYAN_H
#define SCENE_RYAN_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"

class SceneRyan : public Scene
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
		GEO_SHARKTOP,
		GEO_SHARKBTM,
		GEO_BEACH,
		GEO_MINIGUN,
		GEO_TEXT,
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

	float rotateshark;

	bool rotate;
	float rotatetail;
	int sharkcircle;
	float sharkcircleangle;
	bool sharkattack;
	double temptime;

	float sharkdir;
	int Tempcounter;

	Mesh* meshList[NUM_GEOMETRY];


	//trying chasing code
	Vector3 Temptarget;

	//MINIGUN
	float Shootingspin;
	Mesh* MinigunHold;
	Vector3 GunOrigin;
	void Minigun();
	//NOT FOR MY SCENE


	Light light[2];

	Camera3 camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, Color color, float size, float x, float y);
public:

	SceneRyan();
	~SceneRyan();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Update(double dt, Mouse mouse);
	virtual void Render();
	virtual void Exit();
	void RenderShark();
	Color RED = Color(1.f, 0.f, 0.f);
	Color GREEN = Color(0.f, 1.f, 0.f);
	Color BLUE = Color(0.f, 0.f, 1.f);
	Color WHITE = Color(1.f, 1.f, 1.f);
	Color YELLOW = Color(1.f, 1.f, 0.f);
	Color PURPLE = Color(1.f, 0.f, 1.f);
	Color CYAN = Color(0.f, 1.f, 1.f);
	Color BROWN = Color(.6f, .1f, .2f);
	Color BLACK = Color(0.f, 0.f, 0.f);
	Color ORANGE = Color((195 / 255), (82 / 255), (20 / 255));
	Color GRAY = Color(0.4f, 0.4f, 0.4f);

	MS modelStack, viewStack, projectionStack;
};

#endif