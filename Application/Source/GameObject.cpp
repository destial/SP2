#include "GameObject.h"
#include <iostream>

GameObject::GameObject(Mesh* mesh) {
	this->mesh = mesh;
	this->transform = &mesh->transform;
	this->camera = false;
	id = 0;
}

GameObject::GameObject(Mesh* mesh, Transform& transformation) {
	this->mesh = mesh;
	this->transform = &transformation;
	this->camera = false;
	id = 0;
}

GameObject::~GameObject() {}
