#pragma once
#include "Mesh.h"
class GameObject {
public:
	enum OBJECT_TYPE {
		BULLET,
		CAR,
		BUILDING,
		WALL,
		ENTITY,
		UNKNOWN
	};
	GameObject();
	GameObject(Mesh* mesh);
	GameObject(Mesh* mesh, Transform& transformation);
	~GameObject();
	Mesh* mesh;
	Transform* transform;
	Vector3 right, up, view, target;
	int id;
	bool camera;
	int type;
};