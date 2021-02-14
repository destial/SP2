#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3() {}

Camera3::~Camera3() {}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float& fov) {
	this->position = pos;
	this->target = target;
	this->fov = fov;
	this->up = up;
	jumpFrame = 0;
}

void Camera3::Reset() {}

void Camera3::Update(double &dt, Mouse& mouse) {
	const float SENSITIVITY = 0.08f;
	Vector3 view = (target - position).Normalized();

	if (mouse.left) {
		Mtx44 rotation;
		rotation.SetToRotation((mouse.x_diff * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		target = position + view;
	} else if (mouse.right) {
		Mtx44 rotation;
		rotation.SetToRotation((-mouse.x_diff * SENSITIVITY), up.x, up.y, up.z);
		view = (rotation * view).Normalized();
		target = position + view;
	}

	view = (target - position).Normalized();
	Vector3 right = view.Cross(up).Normalized();
	right.y = 0;

	if (mouse.down) {
		Mtx44 rotation;
		rotation.SetToRotation((-mouse.y_diff * SENSITIVITY), right.x, right.y, right.z);
		view = (rotation * view).Normalized();
		target = position + view;
	} else if (mouse.up) {
		Mtx44 rotation;
		rotation.SetToRotation((mouse.y_diff * SENSITIVITY), right.x, right.y, right.z);
		view = (rotation * view).Normalized();
		target = position + view;
	}

	view = (target - position).Normalized();
	right.y = 0;
	up = right.Cross(view).Normalized();
	if (up.y < 0.f) {
		target.y += 0.1f;
		view = (target - position).Normalized();
		right.y = 0;
		up = right.Cross(view).Normalized();
	}
	if (position != target)
		fov += fov * mouse.scroll * SENSITIVITY;

	float boundary = 35.f;

	if (Application::IsKeyPressed('W')) {
		bool hit = false;
		if (position.x <= boundary && position.x >= -boundary) {
			position.x += view.x * SENSITIVITY;
			target.x += view.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z += view.z * SENSITIVITY;
			target.z += view.z * SENSITIVITY;
		} 
		if (position.x < -boundary || position.x > boundary) {
			position.x = (position.x < -boundary ? -boundary : boundary);
		}
		if (position.z < -boundary || position.z > boundary) {
			position.z = (position.z < -boundary ? -boundary : boundary);
		}
	}

	if (Application::IsKeyPressed('S')) {
		if (position.x <= boundary && position.x >= -boundary) {
			position.x -= view.x * SENSITIVITY;
			target.x -= view.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z -= view.z * SENSITIVITY;
			target.z -= view.z * SENSITIVITY;
		}
		if (position.x < -boundary || position.x > boundary) {
			position.x = (position.x < -boundary ? -boundary : boundary);
		}
		if (position.z < -boundary || position.z > boundary) {
			position.z = (position.z < -boundary ? -boundary : boundary);
		}
	}

	if (Application::IsKeyPressed('A')) {
		if (position.x <= boundary && position.x >= -boundary) {
			position.x -= right.x * SENSITIVITY;
			target.x -= right.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z -= right.z * SENSITIVITY;
			target.z -= right.z * SENSITIVITY;
		}
		if (position.x < -boundary || position.x > boundary) {
			position.x = (position.x < -boundary ? -boundary : boundary);
		}
		if (position.z < -boundary || position.z > boundary) {
			position.z = (position.z < -boundary ? -boundary : boundary);
		}
	}

	if (Application::IsKeyPressed('D')) {
		if (position.x <= boundary && position.x >= -boundary) {
			position.x += right.x * SENSITIVITY;
			target.x += right.x * SENSITIVITY;
		}
		if (position.z <= boundary && position.z >= -boundary) {
			position.z += right.z * SENSITIVITY;
			target.z += right.z * SENSITIVITY;
		}
		if (position.x < -boundary || position.x > boundary) {
			position.x = (position.x < -boundary ? -boundary : boundary);
		}
		if (position.z < -boundary || position.z > boundary) {
			position.z = (position.z < -boundary ? -boundary : boundary);
		}
	}

	if (Application::IsKeyPressed(' ')) {
		if (jumpFrame == 0 && position.y <= 0.5f) {
			jumpFrame++;
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