#include "CameraSceneInsideHouse.h"
#include "Application.h"
#include "Mtx44.h"

CameraSceneInsideHouse::CameraSceneInsideHouse()
{
	orthographic_size = 45.0f;

}

CameraSceneInsideHouse ::~CameraSceneInsideHouse()
{
}

void CameraSceneInsideHouse::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	yaw = -90;
	pitch = 0;
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	this->Decoy = defaultPosition = pos;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void CameraSceneInsideHouse::Update(double dt)
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
		if (Collision(2,36.5,-23.2,13) == true)
		{
			/*Decoy += CameraDirection * WALK_SPEED * dt;*/
			position += CameraDirection * WALK_SPEED * dt;
			target += CameraDirection * WALK_SPEED * dt;
			
		}


	}

	if (Application::IsKeyPressed('A'))
	{
		Decoy-= RightDirection * WALK_SPEED * dt;
		if (Collision(2, 36.5, -23.2, 13) == true)
		{
			/*Decoy += CameraDirection * WALK_SPEED * dt;*/
			position -= RightDirection * WALK_SPEED * dt;
			target -= RightDirection * WALK_SPEED * dt;
		}
	}

	if (Application::IsKeyPressed('S'))
	{
		Decoy -= CameraDirection * WALK_SPEED * dt;
		if (Collision(2, 36.5, -23.2, 13) == true)
		{
			/*Decoy += CameraDirection * WALK_SPEED * dt;*/
			position -= CameraDirection * WALK_SPEED * dt;
			target -= CameraDirection * WALK_SPEED * dt;
		}
	}

	if (Application::IsKeyPressed('D'))
	{
		Decoy += RightDirection * WALK_SPEED * dt;
		if (Collision(2, 36.5, -23.2, 13) == true)
		{
			/*Decoy += CameraDirection * WALK_SPEED * dt;*/
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

void CameraSceneInsideHouse::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

float CameraSceneInsideHouse::getRotation(void) {
	Vector3 line = (target - position);
	Vector3 dir = line.Normalized(); //Line from origin(NORMALISED)

	line.y = 0;
	Vector3 flatdir = line.Normalized(); //Line from origin with no elevation(NORMALISED)

	float dotProd = (dir.x * flatdir.x) + (dir.y * flatdir.y) + (dir.z * flatdir.z);
	float LengthTotal = dir.Length() * flatdir.Length();

	float degreeRad = acos(dotProd / LengthTotal);

	return Math::RadianToDegree(degreeRad);
}

bool CameraSceneInsideHouse::Collision(float x1, float x2, float z1, float z2)
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

float CameraSceneInsideHouse::getCameraFinal(float pitch) {
	float maxRotate = 70.f;
	pitch = fabsf(pitch);
	float rotation = getRotation();

	if (maxRotate < (rotation + pitch)) {
		float offset = (rotation + pitch) - maxRotate; //Diff between max degree and calculated result
		pitch -= offset; //Offset the value
	}

	return Math::Max(0.f, pitch);
}