#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 Decoy;

	float phi;
	float theta;
	float rad;

	Camera();
	~Camera();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt);

private:
	void UpdatePos();
};

#endif