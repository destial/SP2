#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 60.f;
	static const float ZOOM_SPEED = 20.f;
	if(Application::IsKeyPressed('A'))
	{
		float yaw = -CAMERA_SPEED * static_cast<float>(dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if(Application::IsKeyPressed('D'))
	{
		float yaw = CAMERA_SPEED * static_cast<float>(dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if(Application::IsKeyPressed('W'))
	{
		float pitch = -CAMERA_SPEED * static_cast<float>(dt);
		if (position.y > 0) {
			pitch = -getCameraFinal(pitch);
		}

		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if(Application::IsKeyPressed('S'))
	{
		float pitch = CAMERA_SPEED * static_cast<float>(dt);
		if (position.y < 0) {
			pitch = getCameraFinal(pitch);
		}

		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		Vector3 view = target - position;
		if(view.Length() > 5) //Prevent the camera from touching the origin
		{
			view.Normalize();
			position += view * ZOOM_SPEED * float(dt);
		}
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * ZOOM_SPEED * float(dt);
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

float Camera2::getRotation(void) {
	Vector3 line = (target - position);
	Vector3 dir = line.Normalized(); //Line from origin(NORMALISED)

	line.y = 0;
	Vector3 flatdir = line.Normalized(); //Line from origin with no elevation(NORMALISED)

	float dotProd = (dir.x * flatdir.x) + (dir.y * flatdir.y) + (dir.z * flatdir.z);
	float LengthTotal = dir.Length() * flatdir.Length();

	float degreeRad = acos(dotProd / LengthTotal);

	return Math::RadianToDegree(degreeRad);
}

float Camera2::getCameraFinal(float pitch) {
	float maxRotate = 70.f;
	pitch = fabsf(pitch);
	float rotation = getRotation();

	if (maxRotate < (rotation + pitch)) {
		float offset = (rotation + pitch) - maxRotate; //Diff between max degree and calculated result
		pitch -= offset; //Offset the value
	}

	return Math::Max(0.f, pitch);
}