#pragma once
#include "Vector3.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

struct MinigameEntity
{
	Vector3 pos;
	Vector3 tempPos;
	Vector3 dHitbox[2]; //not to be done for now
	Vector3 uHitbox[2]; //not to be done for now

	std::string name;

	bool isMoving = false;
	bool isCollided = false;
	float timemoved = 0; //calculate how long the target move;
	int DirectionDummy = 0;

	void MoveDummy(double dt) 
	{
		switch
			(DirectionDummy) 
		{
		case 1: //front
			pos.x += 10 * dt;
			break;
		case 2: //back
			pos.x -= 10 * dt;
			break;
		case 3: //left
			pos.z -= 10 * dt;
			break;
		case 4: //right
			pos.z += 10 * dt;
			break;
		case 5: //up
			pos.y += 5 * dt;
			break;
		case 6: //down
			pos.y -= 5 * dt;
		}
		
	timemoved += dt;
	}
};