#pragma once
#include "Vector3.h"
#include <vector>
#include <iostream>

struct MinigameEntity
{
	Vector3 pos;
	Vector3 tempPos;
	Vector3 dHitbox[2]; //not to be done for now
	Vector3 uHitbox[2]; //not to be done for now

	std::string name;

	bool isPoint;
	bool isMoving = false;
	bool isUp;
	bool isCollided = false;
};