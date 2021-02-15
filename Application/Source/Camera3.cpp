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

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	yaw = -90;
	pitch = 0;
	jumpFrame = 0;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->Decoy = defaultPosition = pos;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = up;
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 70.f;
	static const float ZOOM_SPEED = 20.f;
	static const float WALK_SPEED = 30.f;
	Vector3 CameraDirection = (target - position).Normalized();
	Vector3 RightDirection = CameraDirection.Cross(up);
	if (canfly == false)
	{
		CameraDirection.y = 0;
		CameraDirection.Normalize();
	}

	if (Application::IsKeyPressed('C') and justpress == false)
	{
		canfly = !canfly;
		justpress = !justpress;
	
	}
	else if (!Application::IsKeyPressed('C') and justpress == true)
	{
		justpress = !justpress;
	}

	if (Application::IsKeyPressed('W'))
	{
		Decoy += CameraDirection * WALK_SPEED * dt;
			//house 1										   //house 2
		if (Collision(-0.0238, 37.7, -28.6, 23.8) == false and Collision(-111, -67.2, -35.2, 34.8) == false and
			Collision(-188,129,168,208) == false and Collision(75.8,100,-193,170) == false and 
			Collision(-199,161,-162,-108.6) == false and Collision(-181,-143,-201,189) == false)
		{
			//if (Collision(-0.0238, 37.7, -28.6, 23.8) == false)
			//{
				position += CameraDirection * WALK_SPEED * dt;
				target += CameraDirection * WALK_SPEED * dt;
			//}
		}
	}

	if (Application::IsKeyPressed('A'))
	{
		Decoy-= RightDirection * WALK_SPEED * dt;
		if (Collision(-0.0238, 37.7, -28.6, 23.8) == false and Collision(-111, -67.2, -35.2, 34.8) == false and
			Collision(-188, 129, 168, 208) == false and Collision(75.8, 100, -193, 170) == false and
			Collision(-199, 161, -162, -108.6) == false and Collision(-181, -143, -201, 189) == false)
		{
			position -= RightDirection * WALK_SPEED * dt;
			target -= RightDirection * WALK_SPEED * dt;
		}
	}

	if (Application::IsKeyPressed('S'))
	{
		Decoy -= CameraDirection * WALK_SPEED * dt;
		if (Collision(-0.0238, 37.7, -28.6, 23.8) == false and Collision(-111, -67.2, -35.2, 34.8) == false and
			Collision(-188, 129, 168, 208) == false and Collision(75.8, 100, -193, 170) == false and
			Collision(-199, 161, -162, -108.6) == false and Collision(-181, -143, -201, 189) == false)
		{
			position -= CameraDirection * WALK_SPEED * dt;
			target -= CameraDirection * WALK_SPEED * dt;
		}
	}

	if (Application::IsKeyPressed('D'))
	{
		Decoy += RightDirection * WALK_SPEED * dt;
		if (Collision(-0.0238, 37.7, -28.6, 23.8) == false and Collision(-111, -67.2, -35.2, 34.8) == false and
			Collision(-188, 129, 168, 208) == false and Collision(75.8, 100, -193, 170) == false and
			Collision(-199, 161, -162, -108.6) == false and Collision(-181, -143, -201, 189) == false)
		{
			position += RightDirection * WALK_SPEED * dt; 
			target += RightDirection * WALK_SPEED * dt;
		}
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		pitch += CAMERA_SPEED * dt;
		if (pitch > 85)
		{
			pitch = 85;
		}
	}
	if(Application::IsKeyPressed(VK_LEFT))
	{
		yaw -= CAMERA_SPEED * dt;
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		pitch -= CAMERA_SPEED * dt;
		if (pitch < -85)
		{
			pitch = -85;
		}
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		yaw += CAMERA_SPEED * dt;
	}
	if(Application::IsKeyPressed('Z'))
	{
		orthographic_size -= ZOOM_SPEED * dt;
		if (orthographic_size < 1)
		{
			orthographic_size = 1;
		}
		
	}
	if(Application::IsKeyPressed('X'))
	{
		orthographic_size += ZOOM_SPEED * dt;
		if (orthographic_size > 100)
		{
			orthographic_size = 100;
		}
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
	Vector3 view;
	Vector3 right;

	if (yaw > 360)
	{
		yaw -= 360;
	}
	else if (yaw < 0)
	{
		yaw += 360;
	}

	target.x = cos(Math::DegreeToRadian(yaw)) * cos(Math::DegreeToRadian(pitch));
	target.y = sin(Math::DegreeToRadian(pitch));
	target.z = sin(Math::DegreeToRadian(yaw)) * cos(Math::DegreeToRadian(pitch));

	view = target;
	right = view.Cross(Vector3(0, 1, 0)).Normalized();
	up = right.Cross(view).Normalized();
	target += position;
}

void Camera3::Update(double& dt, Mouse& mouse) {
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
	up = right.Cross(view).Normalized();
	if (up.y < 0.f) {
		target.y += 0.1f;
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

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

float Camera3::getRotation(void) {
	Vector3 line = (target - position);
	Vector3 dir = line.Normalized(); //Line from origin(NORMALISED)

	line.y = 0;
	Vector3 flatdir = line.Normalized(); //Line from origin with no elevation(NORMALISED)

	float dotProd = (dir.x * flatdir.x) + (dir.y * flatdir.y) + (dir.z * flatdir.z);
	float LengthTotal = dir.Length() * flatdir.Length();

	float degreeRad = acos(dotProd / LengthTotal);

	return Math::RadianToDegree(degreeRad);
}

bool Camera3::Collision(float x1, float x2, float z1, float z2)
{
	if (Decoy.x > x1 and Decoy.x < x2 and Decoy.z > z1 and Decoy.z < z2)
	{
		Decoy.x = position.x;
		Decoy.z = position.z;
		return true;
	}
	else
	{
		return false;
	}
}

float Camera3::getCameraFinal(float pitch) {
	float maxRotate = 70.f;
	pitch = fabsf(pitch);
	float rotation = getRotation();

	if (maxRotate < (rotation + pitch)) {
		float offset = (rotation + pitch) - maxRotate; //Diff between max degree and calculated result
		pitch -= offset; //Offset the value
	}

	return Math::Max(0.f, pitch);
}