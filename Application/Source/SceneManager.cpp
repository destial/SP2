#include "SceneManager.h"

SceneManager::SceneManager(Scene* scene, float bounds) {
	root = new Quad(bounds);
	this->scene = scene;
	totalObjects = 0;
}
SceneManager::~SceneManager() {
	root->deleteAll();
	allObjects.clear();
	deleteAllQuad(root);
}

void SceneManager::split(Quad* parent) {
	if (parent->count > 5) {
		Quad* upL = new Quad(parent, Quad::ENUM_QUADTYPE::E_UPL);
		parent->setUpL(upL);
		split(upL);

		Quad* upR = new Quad(parent, Quad::ENUM_QUADTYPE::E_UPR);
		parent->setUpR(upR);
		split(upR);

		Quad* botL = new Quad(parent, Quad::ENUM_QUADTYPE::E_BOTL);
		parent->setBotL(botL);
		split(botL);

		Quad* botR = new Quad(parent, Quad::ENUM_QUADTYPE::E_BOTR);
		parent->setBotR(botR);
		split(botR);
	}
}

void SceneManager::deleteAllQuad(Quad* parent) {
	if (parent) {
		deleteAllQuad(parent->upL);
		deleteAllQuad(parent->upR);
		deleteAllQuad(parent->botL);
		deleteAllQuad(parent->botR);
		delete parent;
	}
}

Quad* SceneManager::getQuad(int gameObjectID) {
	if (root && root->count != 0) {
		Quad* temp = root;
		while (temp->upL) {
			temp = temp->upL;
		}
		if (temp->hasGameObject(gameObjectID)) return temp;
		temp = root;
		while (temp->upR) {
			temp = temp->upR;
		}
		if (temp->hasGameObject(gameObjectID)) return temp;
		temp = root;
		while (temp->botL) {
			temp = temp->botL;
		}
		if (temp->hasGameObject(gameObjectID)) return temp;
		temp = root;
		while (temp->botR) {
			temp = temp->botR;
		}
		if (temp->hasGameObject(gameObjectID)) return temp;
	}
	return nullptr;
}

void SceneManager::push(GameObject* gameObject) {
	root->push(gameObject);
	++root->count;
	allObjects.push_back(gameObject);
	totalObjects = (allObjects.size() > root->gameObjects.size() ? allObjects.size() : root->gameObjects.size());
}