#ifndef SCENE_H
#define SCENE_H

#include "Application.h"

class Scene {
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Update(double dt, Mouse mouse) = 0;
	virtual void InitGL() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif