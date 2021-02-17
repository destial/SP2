#ifndef CAMERA_3_H
#define CAMERA_3_H
#include "Vector3.h"
#include "Application.h"
class Camera3
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 Decoy;
	Vector3 defaultUp;

	Vector3 carView, carTarget;
	Vector3 prevTarget, prevPosition, prevUp;
	Vector3 target, position, up;

	Camera3();
	~Camera3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& bounds);
	void Update(double dt);
	void Update(double& dt, Mouse &mouse);
	void UpdateCar(double& dt, Mouse& mouse, const float& SPEED);
	void Reset();
	float yaw;
	float pitch;
	float orthographic_size;
	float defaultFOV;
	float prevFOV;
	float bounds;
	float sprintRotation;
	float currentCarSpeed;
	bool canfly = false;
	bool justpress = false;
	bool sprintRotate = false;
	bool inverted = false;
	//My own stuff
	float getRotation(void);
	float getCarRotation();
	float getRotation(Vector3& origin);
	float getCarRotation(Vector3& carOrigin);
	bool invert();
	float jumpFrame;
};

#endif