#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Vector3.h"
#include "Application.h"

class Camera3 {
public:
	Camera3();
	~Camera3();

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 Decoy;
	Vector3 defaultUp;

	Vector3 carView, carTarget;
	Vector3 prevTarget, prevPosition, prevUp;
	Vector3 target, position, up, right;

	//SharkVectors
	Vector3 SharkView, SharkTarget;
	Vector3 SharkRight;
	Vector3 SharkFace;
	Vector3 SharkPos;
	//BulletVectors
	Vector3 BulletView, BulletTarget;
	Vector3 BulletRight;
	Vector3 BulletFace;
	Vector3 BulletPos;

	float yaw;
	float pitch;
	float orthographic_size;
	float rightY;
	float defaultFOV;
	float prevFOV;
	float bounds;
	float sprintRotation;
	float currentCarSpeed;
	float jumpFrame;
	bool canfly = false;
	bool justpress = false;
	bool sprintRotate = false;
	bool inverted = false;

	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& bounds);
	void Update(double dt);
	void Update(double& dt, Mouse& mouse);
	void UpdateFlying(double& dt, Mouse& mouse);
	void UpdateCar(double& dt, Mouse& mouse, const float& SPEED);
	void Reset();
	//Shark stuff
	void SharkChaseinit();
	float getSharkRotation(Vector3& origin);
	void SharkChaseMove();
	//Bullet stuff
	void Bulletinit();
	void BulletMove();



	float getRotation(void);
	float getCarRotation();
	float getRotation(Vector3& origin);
	float getCarRotation(Vector3& carOrigin);
	bool invert();
};

#endif