#pragma once
#include "Mesh.h"
class GameObject {
public:
	GameObject(Mesh* mesh);
	GameObject(Mesh* mesh, Transform& transformation);
	~GameObject();
	Mesh* mesh;
	Transform* transform;
	Vector3 right, up, view;
	int id;
	bool camera;
};