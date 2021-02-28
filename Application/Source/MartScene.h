#ifndef SCENE_SHAQLER_H
#define SCENE_SHAQLER_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"

class MartScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_QUAD1,
		GEO_QUAD2,
		GEO_QUAD3,
		GEO_QUAD4,
		GEO_QUAD5,
		GEO_QUAD6,
		GEO_SPHERE,
		GEO_TREASURECHEST,
		GEO_DESKCORNER,
		GEO_COMPUTERSCREEN,
		GEO_KEYBOARD,
		GEO_TABLEANDCHAIR,
		GEO_STATUE,
		GEO_SOFA,
		GEO_TABLE,
		GEO_BOOKSTACK,
		GEO_BOOK,
		GEO_BOOK2,
		GEO_BOOK3,
		GEO_BOOK4,
		GEO_BOOK5,
		GEO_BOOK6,
		GEO_BOOK7,
		GEO_BOOK8,
		GEO_CHAIR,
		GEO_CUBE,
		GEO_PAINTING,
		GEO_BOOKCASE,
		GEO_KURENAI,
		GEO_SAMIDAL,
		GEO_SAMIDALLEFTARM,
		GEO_SAMIDALRIGHTARM,
		GEO_UI,
		GEO_TRASHCAN,
		GEO_DOOR,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
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

	enum SCENE_FLOATS {
		// similar to my first scene where there is floats for movements
		F_ROTATE_BOOK,
		F_ROTATE_DOOR,
		F_AMOUNT,
		F_X,
		NUM_SCENE_FLOATS
	};

	enum SCENE_VECTORS {
		V_BOOK,
		V_TEXT,
		V_SCREEN,
		V_SCREEN_TEXT_1,
		V_SCREEN_TEXT_2,
		V_TEXT_WORLD_SCREEN,
		V_BOOK_ORIGIN,
		NUM_SCENE_VECTORS
	};

	enum SCENE_BOOLS {
		// bool for movement limit and begin movement/interactions with npc and objects
		B_ROTATE,
		B_HEIGHT_LIMIT,
		B_BOOK_COLLECTED,
		B_PURCHASE_BOOK,
		B_BOOK_BOUGHT,
		B_DOOR_OPENED,
		B_STOP_OPEN_DOOR,
		B_IS_BUYING,
		B_FINDMONEY,
		B_CASHCOLLECTED,
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

	Vector3 sceneVectors[NUM_SCENE_VECTORS];
	bool sceneBools[NUM_SCENE_BOOLS];
	float sceneFloats[NUM_SCENE_FLOATS];
	Mesh* meshList[NUM_GEOMETRY];
	Mesh* BookHold;
	Light light[2];
	Camera3 camera;
	int GLcounter;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float size, float x, float y);

	void RenderWalls();
	void RenderInatimateobjects();
	void RenderText();
	void RenderNPC();
	void RenderBooks();
	void RenderBooks2();
	void Book();
	void RenderUI();
	void RenderSkybox();

public:
	MartScene();
	~MartScene();

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