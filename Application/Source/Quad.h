#pragma once
#include "GameObject.h"
#include <vector>
class Quad {
public:
	enum ENUM_QUADTYPE {
		E_UPL = 0,
		E_UPR,
		E_BOTL,
		E_BOTR,
		E_ROOT
	};
	Quad(float bounds);
	Quad(Quad* parent, int type);
	~Quad();
	Quad* parent;
	Quad* upL, *upR, *botL, *botR;
	std::vector<GameObject*> gameObjects;
	unsigned count, height, width, type, level;
	Vector3 origin;

	void deleteAll();
	void setUpL(Quad* quad);
	void setUpR(Quad* quad);
	void setBotL(Quad* quad);
	void setBotR(Quad* quad);
	bool hasGameObject(int id);
};

