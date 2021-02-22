#pragma once
#include "Quad.h"
#include "Scene.h"

class SceneManager {
public:
	SceneManager(Scene* scene, float bounds);
	~SceneManager();
	Scene* scene;
	Quad* root;
	unsigned totalObjects;
	std::vector<GameObject*> allObjects;
	void split(Quad* parent);
	void deleteAllQuad(Quad* parent);
	Quad* getQuad(int gameObjectID);
	void push(GameObject* gameObject);
};