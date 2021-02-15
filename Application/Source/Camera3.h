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

	Vector3 target, position, up;

	Camera3();
	~Camera3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& bounds);
	void Update(double dt);
	void Update(double& dt, Mouse &mouse);
	void Reset();
	float yaw;
	float pitch;
	float orthographic_size;
	float bounds;
	bool canfly = false;
	bool justpress = false;
	//My own stuff
	float getRotation(void);
private:
	float jumpFrame;
};

#endif