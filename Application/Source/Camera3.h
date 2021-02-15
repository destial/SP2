#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "Application.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& bounds);
	virtual void Update(double dt);
	virtual void Update(double &dt, Mouse& mouse);
	virtual void Reset();
	float yaw;
	float pitch;
	float orthographic_size;
	float bounds;
	bool canfly = false;
	bool justpress = false;
	//My own stuff
	float getRotation(void);
	bool Collision(float x1, float x2, float z1, float z2);
private:
	float getCameraFinal(float pitch); //get the camera's final rotation
	float jumpFrame;
};

#endif