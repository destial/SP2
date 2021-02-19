#include "GameObject.h"
#include <iostream>

GameObject::GameObject(Mesh* mesh) {
	this->mesh = mesh;
	this->transform = new Transform();
	this->camera = false;
	id = 0;
}

GameObject::GameObject(Mesh* mesh, Transform& transformation) {
	this->mesh = mesh;
	this->transform = new Transform(transformation);
	this->camera = false;
	id = 0;
}

GameObject::~GameObject() {
	delete transform;
}
