#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3() {
	orthographic_size = defaultFOV = 45.0f;
}

Camera3::~Camera3() {}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float &bounds) {
	yaw = -90;
	pitch = jumpFrame = sprintRotation = currentCarSpeed = 0;
	this->bounds = bounds;
	this->position = defaultPosition = Decoy = pos;
	this->target = defaultTarget = carTarget = target;
	this->up = defaultUp = up;
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up) {
	yaw = -90;
	pitch = jumpFrame = sprintRotation = currentCarSpeed = 0;
	this->bounds = 50.f;
	this->position = defaultPosition = Decoy = pos;
	this->target = defaultTarget = carTarget = target;
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
	float SENSITIVITY = 0.2f;

	SENSITIVITY = (inverted ? -SENSITIVITY : SENSITIVITY);

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
	right = view.Cross(up).Normalized();
	right.y = 0;
	up = right.Cross(view).Normalized();

	if (up.y < 0.f) {
		target.y += 1.f;
		view = (target - position).Normalized();
		right.y = 0;
		up = right.Cross(view).Normalized();
	}

	if (mouse.scroll != 0) {
		orthographic_size += mouse.scroll * SENSITIVITY * 5.f;
	}

	if (orthographic_size > 100)
		orthographic_size = 100;
	if (orthographic_size < 1)
		orthographic_size = 1;

	prevFOV = orthographic_size;
	currentCarSpeed = 0;

	float SPRINT = 1.f;

	if (Application::IsKeyPressed(VK_LSHIFT) && Application::IsKeyPressed('W')) {
		if (sprintRotation < -10.f) {
			sprintRotate = 1;
		} else if (sprintRotation > 10.f) {
			sprintRotate = 0;
		}
		if (sprintRotate) {
			sprintRotation += 1.f;
		} else {
			sprintRotation -= 1.f;
		}
		SPRINT *= 2.f;
		Mtx44 rotation;
		rotation.SetToRotation(sprintRotation, view.x, 0, view.z);
		view = (rotation * view).Normalized();
		target = position + view;
	} else {
		sprintRotation = 0;
	}

	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();
	right.y = 0;
	up = right.Cross(view).Normalized();

	if (Application::IsKeyPressed('W')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= bounds && position.x >= -bounds) {
			position.x += face.x * SENSITIVITY * SPRINT;
			target.x += face.x * SENSITIVITY * SPRINT;
		}
		if (position.z <= bounds && position.z >= -bounds) {
			position.z += face.z * SENSITIVITY * SPRINT;
			target.z += face.z * SENSITIVITY * SPRINT;
		}
		if (position.x <= -bounds || position.x >= bounds) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -bounds || position.z >= bounds) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('S')) {
		Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= bounds && position.x >= -bounds) {
			position.x -= face.x * SENSITIVITY;
			target.x -= face.x * SENSITIVITY;
		}
		if (position.z <= bounds && position.z >= -bounds) {
			position.z -= face.z * SENSITIVITY;
			target.z -= face.z * SENSITIVITY;
		}
		if (position.x <= -bounds || position.x >= bounds) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -bounds || position.z >= bounds) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('A')) {
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x <= bounds && position.x >= -bounds) {
			position.x -= right.x * SENSITIVITY;
			target.x -= right.x * SENSITIVITY;
		}
		if (position.z <= bounds && position.z >= -bounds) {
			position.z -= right.z * SENSITIVITY;
			target.z -= right.z * SENSITIVITY;
		}
		if (position.x <= -bounds || position.x >= bounds) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -bounds || position.z >= bounds) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('D')) {
		Vector3 oldPos = position;
		Vector3 oldTar = target;
		if (position.x < bounds && position.x > -bounds) {
			position.x += right.x * SENSITIVITY;
			target.x += right.x * SENSITIVITY;
		}
		if (position.z < bounds && position.z > -bounds) {
			position.z += right.z * SENSITIVITY;
			target.z += right.z * SENSITIVITY;
		}
		if (position.x <= -bounds || position.x >= bounds) {
			position.x = oldPos.x;
			target.x = oldTar.x;
		}
		if (position.z <= -bounds || position.z >= bounds) {
			position.z = oldPos.z;
			target.z = oldTar.z;
		}
	}

	if (Application::IsKeyPressed('E')) {

	}

	if (Application::IsKeyPressed(' ')) {
		if (position.y <= bounds) {
			if (position.y == defaultPosition.y) {
				jumpFrame++;
			}
		}
	}

	const float JUMP_SPEED = SENSITIVITY * 0.5f;

	if (jumpFrame != 0 && jumpFrame < 25) {
		position.y += JUMP_SPEED;
		target.y += JUMP_SPEED;
		jumpFrame++;
	} else if (jumpFrame == 0) {

	} else {
		jumpFrame = 0;
	}

	if (jumpFrame == 0) {
		if (position.y > defaultPosition.y) {
			position.y -= JUMP_SPEED;
			target.y -= JUMP_SPEED;
			if (position.y < defaultPosition.y) {
				float diff = defaultPosition.y - position.y;
				position.y = defaultPosition.y;
				target.y += diff;
			}
		}
		if (position.y < defaultPosition.y) {
			position.y = defaultPosition.y;
		}
	}
}

void Camera3::UpdateCar(double& dt, Mouse& mouse, const float& SPEED) {
	prevTarget = target;
	prevPosition = position;
	prevUp = up;
	jumpFrame = 0;
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
	orthographic_size = 100.f;

	float boundary = bounds;
	view = (carTarget - position).Normalized();
	right = view.Cross(Vector3(0, 1, 0)).Normalized();
	Vector3 oldCarPos = position;

	if (Application::IsKeyPressed('W')) {
		if (currentCarSpeed != SPEED) {
			currentCarSpeed += 0.05f;
		}
		if (currentCarSpeed > SPEED) {
			currentCarSpeed = SPEED;
		}
	}
	if (Application::IsKeyPressed('S')) {
		if (currentCarSpeed != -(SPEED / 2)) {
			currentCarSpeed -= 0.05f;
		}
		if (currentCarSpeed < -(SPEED / 2)) {
			currentCarSpeed = -(SPEED / 2);
		}
	}
	if (!Application::IsKeyPressed('W') && !Application::IsKeyPressed('S')) {
		bool forward = true;
		if (currentCarSpeed < 0) {
			forward = false;
		}
		if (forward) {
			if (currentCarSpeed != 0) {
				currentCarSpeed -= 0.015f;
			}
			if (currentCarSpeed < 0) {
				currentCarSpeed = 0;
			}
		} else {
			if (currentCarSpeed != 0) {
				currentCarSpeed += 0.015f;
			}
			if (currentCarSpeed > 0) {
				currentCarSpeed = 0;
			}
		}
	}
	Vector3 face = Vector3(0, 1, 0).Cross(right).Normalized();
	Vector3 oldPos = position;
	Vector3 oldTar = target;
	Vector3 oldCar = carTarget;
	if (position.x <= boundary && position.x >= -boundary) {
		position.x += face.x * SENSITIVITY * currentCarSpeed;
		target.x += face.x * SENSITIVITY * currentCarSpeed;
		carTarget.x += face.x * SENSITIVITY * currentCarSpeed;
	}
	if (position.z <= boundary && position.z >= -boundary) {
		position.z += face.z * SENSITIVITY * currentCarSpeed;
		target.z += face.z * SENSITIVITY * currentCarSpeed;
		carTarget.z += face.z * SENSITIVITY * currentCarSpeed;
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

	view = (carTarget - position).Normalized();

	if (position == oldCarPos) return;

	if (Application::IsKeyPressed('A')) {
		Mtx44 rotation;
		rotation.SetToRotation((2*SPEED * SENSITIVITY), 0, 1, 0);
		view = (rotation * view).Normalized();
		carTarget = position + view;
	}

	if (Application::IsKeyPressed('D')) {
		Mtx44 rotation;
		rotation.SetToRotation((-2*SPEED * SENSITIVITY), 0, 1, 0);
		view = (rotation * view).Normalized();
		carTarget = position + view;
	}
}

void Camera3::Reset() {
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	carTarget = target;
	orthographic_size = defaultFOV;
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

	// Get the view vector of the car, set to 0 elevation
	Vector3 lookAt = (carTarget - position).Normalized();
	lookAt.y = 0;

	// Use the default origin
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

float Camera3::getCarRotation(Vector3& origin) {

	// Get the view vector of the car, set to 0 elevation
	Vector3 lookAt = (carTarget - position).Normalized();
	lookAt.y = 0;

	// 
	float uv = (origin.x * lookAt.x + origin.z * lookAt.z) / ((Math::sqrt(origin.x * origin.x + origin.z * origin.z)) * Math::sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z));
	float angle = Math::RadianToDegree(acos(uv));
	if (lookAt.z <= 0 && lookAt.x <= 0) {
		angle = 360 - angle;
	} else if (lookAt.x >= 0 && lookAt.z <= 0) {
		angle = 360 - angle;
	}
	return angle;
}

bool Camera3::invert() {
	inverted = (inverted ? 0 : 1);
	return inverted;
}

float Camera3::getRotation(Vector3& origin) {
	Vector3 lookAt = (target - position).Normalized();
	lookAt.y = 0;
	float uv = (origin.x * lookAt.x + origin.z * lookAt.z) / ((Math::sqrt(origin.x * origin.x + origin.z * origin.z)) * Math::sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z));
	float angle = Math::RadianToDegree(acos(uv));
	if (lookAt.z <= 0 && lookAt.x <= 0) {
		angle = 360 - angle;
	} else if (lookAt.x >= 0 && lookAt.z <= 0) {
		angle = 360 - angle;
	}
	return angle;
}
