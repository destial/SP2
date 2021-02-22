#include "Quad.h"

Quad::Quad(float bounds) {
	parent = upL = upR = botL = botR = nullptr;
	count = level = 0;
	type = E_ROOT;
	height = width = 2 * bounds;
	origin = Vector3(-bounds, 0, -bounds);
}

Quad::Quad(Quad* parent, int type) {
	upL = upR = botL = botR = nullptr;
	this->parent = parent;
	height = (parent->height >> 1);
	width = (parent->width >> 1);
	level = parent->level + 1;
	this->type = type;
	count = 0;
	if (parent->height % 2 == 1 && parent->width % 2 == 1) {
		switch (type) {
		case E_UPR:
			width += 1;
			break;
		case E_BOTL:
			height += 1;
			break;
		case E_BOTR:
			height += 1;
			width += 1;
			break;
		default:
			break;
		}
	}
}

Quad::~Quad() {}

void Quad::setUpL(Quad* quad) {
	upL = quad;
	if (upL) {
		upL->origin = this->origin;
		for (auto object : gameObjects) {
			if (object) {
				if (object->transform->translate.x >= upL->origin.x &&
					object->transform->translate.z >= upL->origin.y &&
					object->transform->translate.x < upL->origin.x + upL->width &&
					object->transform->translate.z < upL->origin.z + upL->height) {
					upL->push(object);
				}
			}
		}
	}
}

void Quad::setUpR(Quad* quad) {
	upR = quad;
	if (upR) {
		upR->origin.x = this->origin.x + upL->width;
		upR->origin.z = this->origin.z;
		for (auto object : gameObjects) {
			if (object) {
				if (object->transform->translate.x >= upR->origin.x &&
					object->transform->translate.z >= upR->origin.y &&
					object->transform->translate.z < upR->origin.z + upR->height) {
					upR->push(object);
				}
			}
		}
	}
}

void Quad::setBotL(Quad* quad) {
	botL = quad;
	if (botL) {
		botL->origin.x = this->origin.x;
		botL->origin.z = this->origin.z + upL->height;
		for (auto object : gameObjects) {
			if (object) {
				if (object->transform->translate.x >= botL->origin.x &&
					object->transform->translate.z >= botL->origin.y &&
					object->transform->translate.x < botL->origin.x + botL->width &&
					object->transform->translate.z < botL->origin.z + botL->height) {
					botL->push(object);
				}
			}
		}
	}
}

void Quad::setBotR(Quad* quad) {
	botR = quad;
	if (botR) {
		botR->origin.x = this->origin.x + upL->width;
		botR->origin.z = this->origin.z + upL->height;
		for (auto object : gameObjects) {
			if (object) {
				if (object->transform->translate.x >= botR->origin.x &&
					object->transform->translate.z >= botR->origin.y &&
					object->transform->translate.x < botR->origin.x + botR->width &&
					object->transform->translate.z < botR->origin.z + botR->height) {
					botR->push(object);
				}
			}
		}
	}
}

bool Quad::hasGameObject(int id) {
	for (std::vector<GameObject*>::iterator iterator = gameObjects.begin(); iterator != gameObjects.end(); ++iterator) {
		GameObject* object = *iterator;
		if (object && object->id == id) {
			return true;
		}
	}
	return false;
}

void Quad::push(GameObject* gameObject) {
	gameObjects.push_back(gameObject);
	count = gameObjects.size();
}

void Quad::deleteAll() {
	for (unsigned i = 0; i < gameObjects.size(); ++i) {
		if (gameObjects[i]) delete gameObjects[i];
	}
	gameObjects.clear();
}
