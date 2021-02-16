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
	carTarget = target;
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
	prevTarget = target;
	prevPosition = position;
	prevUp = up;
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

void Camera3::UpdateCar(double& dt, Mouse& mouse, const float& SPEED) {
	prevTarget = target;
	prevPosition = position;
	prevUp = up;
	const float SENSITIVITY = 0.08f;
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
	this->up = right.Cross(view).Normalized();

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
	view = (carTarget - position).Normalized();
	right = view.Cross(Vector3(0, 1, 0)).Normalized();
	if (Application::IsKeyPressed('W')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		Vector3 oldCar = carTarget;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x += face.x * SENSITIVITY * SPEED;
			target.x += face.x * SENSITIVITY * SPEED;
			carTarget.x += face.x * SENSITIVITY * SPEED;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z += face.z * SENSITIVITY * SPEED;
			target.z += face.z * SENSITIVITY * SPEED;
			carTarget.z += face.z * SENSITIVITY * SPEED;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
			carTarget.x = oldCar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
			carTarget.z = oldCar.z;
		}
	}

	if (Application::IsKeyPressed('S')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		Vector3 oldCar = carTarget;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x -= face.x * SENSITIVITY * SPEED;
			target.x -= face.x * SENSITIVITY * SPEED;
			carTarget.x -= face.x * SENSITIVITY * SPEED;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z -= face.z * SENSITIVITY * SPEED;
			target.z -= face.z * SENSITIVITY * SPEED;
			carTarget.z -= face.z * SENSITIVITY * SPEED;
		}
		if (position.x <= -boundary || position.x >= boundary) {
			position.x = oldPos.x;
			target.x = oldTar.x;
			carTarget.x = oldCar.x;
		}
		if (position.z <= -boundary || position.z >= boundary) {
			position.z = oldPos.z;
			target.z = oldTar.z;
			carTarget.z = oldCar.z;
		}
	}

	view = (carTarget - position).Normalized();

	if (Application::IsKeyPressed('A')) {
		Mtx44 rotation;
		rotation.SetToRotation((2 * SPEED * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		carTarget = position + view;
	}

	if (Application::IsKeyPressed('D')) {
		Mtx44 rotation;
		rotation.SetToRotation((-2 * SPEED * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		carTarget = position + view;
	}
}

void Camera3::Reset() {
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	carTarget = target;
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

float Camera3::getCarRotation() {
	Vector3 lookAt = (carTarget - position).Normalized();
	lookAt.y = 0;
	Vector3 origin = Vector3(-1, 0, 0);
	float uv = (origin.x * lookAt.x + origin.z * lookAt.z) / ((Math::sqrt(origin.x * origin.x + origin.z * origin.z)) * Math::sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z));
	float angle = Math::RadianToDegree(acos(uv));
	if (lookAt.z <= 0 && lookAt.x <= 0) {
		angle = 360 - angle;
	} else if (lookAt.x >= 0 && lookAt.z <= 0) {
		angle = 360 - angle;
	}
	return angle;
}
