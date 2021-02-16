#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
	orthographic_size = 45.0f;
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float &bounds)
{
	yaw = -90;
	pitch = 0;
	jumpFrame = 0;
	this->bounds = bounds;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->Decoy = defaultPosition = pos;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = up;
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	yaw = -90;
	pitch = 0;
	jumpFrame = 0;
	this->bounds = 50.f;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->Decoy = defaultPosition = pos;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = up;
}

void Camera3::Update(double dt) {
	Mouse mouse;
	Update(dt, mouse);
}

void Camera3::Update(double& dt, Mouse& mouse) {
	const float SENSITIVITY = 0.2f;
	Vector3 view = (target - position).Normalized();
	if (mouse.left) {
		Mtx44 rotation;
		rotation.SetToRotation((mouse.x * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		target = position + view;
	} else if (mouse.right) {
		Mtx44 rotation;
		rotation.SetToRotation((-mouse.x * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		target = position + view;
	}

	view = (target - position).Normalized();
	Vector3 right = view.Cross(up).Normalized();
	right.y = 0;

	if (mouse.down) {
		Mtx44 rotation;
		rotation.SetToRotation((-mouse.y * SENSITIVITY), right.x, right.y, right.z);
		view = (rotation * view).Normalized();
		target = position + view;
	} else if (mouse.up) {
		Mtx44 rotation;
		rotation.SetToRotation((mouse.y * SENSITIVITY), right.x, right.y, right.z);
		view = (rotation * view).Normalized();
		target = position + view;
	}

	view = (target - position).Normalized();
	right.y = 0;
	up = right.Cross(view).Normalized();
	if (up.y < 0.f) {
		target.y += 1.f;
		view = (target - position).Normalized();
		right.y = 0;
		up = right.Cross(view).Normalized();
	}
	if (position != target)
		orthographic_size += orthographic_size * mouse.scroll * SENSITIVITY;
	if (orthographic_size > 100) 
		orthographic_size = 100;
	if (orthographic_size < 1)
		orthographic_size = 1;

	float boundary = bounds;

	if (Application::IsKeyPressed('W')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x += face.x * SENSITIVITY;
			target.x += face.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z += face.z * SENSITIVITY;
			target.z += face.z * SENSITIVITY;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('S')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x -= face.x * SENSITIVITY;
			target.x -= face.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z -= face.z * SENSITIVITY;
			target.z -= face.z * SENSITIVITY;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('A')) {
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x -= right.x * SENSITIVITY;
			target.x -= right.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z -= right.z * SENSITIVITY;
			target.z -= right.z * SENSITIVITY;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('D')) {
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x < boundary && position.x > -boundary) {
			position.x += right.x * SENSITIVITY;
			target.x += right.x * SENSITIVITY;
		}
		if (position.z < boundary && position.z > -boundary) {
			position.z += right.z * SENSITIVITY;
			target.z += right.z * SENSITIVITY;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('E')) {

	}

	if (Application::IsKeyPressed(' ')) {
		if (position.y <= boundary) {
			position.y += 1 * SENSITIVITY;
			target.y += 1 * SENSITIVITY;
		}
	}

	if (Application::IsKeyPressed(VK_LSHIFT)) {
		if (position.y >= -boundary) {
			position.y -= 1 * SENSITIVITY;
			target.y -= 1 * SENSITIVITY;
		}
	}
	if (jumpFrame != 0 && jumpFrame < 10) {
		position.y += SENSITIVITY;
		target.y += SENSITIVITY;
		jumpFrame++;
	} else if (jumpFrame == 0) {
		return;
	} else {
		jumpFrame = 0;
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

float Camera3::getRotation(void) {
	Vector3 line = (this->target - position);
	Vector3 dir = line.Normalized(); //Line from origin(NORMALISED)

	line.y = 0;
	Vector3 flatdir = line.Normalized(); //Line from origin with no elevation(NORMALISED)

	float dotProd = (dir.x * flatdir.x) + (dir.y * flatdir.y) + (dir.z * flatdir.z);
	float LengthTotal = dir.Length() * flatdir.Length();

	float degreeRad = acos(dotProd / LengthTotal);

	return Math::RadianToDegree(degreeRad);
}