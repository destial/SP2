#pragma once
#include "Quad.h"
#include "Scene.h"

class SceneManager {
public:
	SceneManager(Scene* scene, float bounds);
	~SceneManager();
	Scene* scene;
	Quad* root;
	void split(Quad* parent);
	void deleteAll(Quad* parent);
	Quad* getQuad(int gameObjectID);
	void push(GameObject* gameObject);
};