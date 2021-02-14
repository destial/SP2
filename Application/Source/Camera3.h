#ifndef CAMERA3_H
#define CAMERA3_H

#include "Vector3.h"
#include "Application.h"
#include <math.h>

class Camera3 {
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	float fov;

	Camera3();
	~Camera3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& fov);
	void Reset();
	void Update(double &dt, Mouse& mouse);
private:
	unsigned jumpFrame;
};

#endif