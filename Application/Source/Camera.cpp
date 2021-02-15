#include "Camera.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructors
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = pos;
    this->target = target;
    this->up = up;

    rad = (float)pow(pow(position.x, 2) + pow(position.y, 2) + pow(position.z, 2), 0.5);

    phi = asin(position.y / rad);

    float cosphi = cos(phi);
    float totaltemp = 10.1980390272f * cosphi;

    theta = acos(position.y / totaltemp);
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
    float spd = 2 * float(dt);

    if (Application::IsKeyPressed('W'))
    {
        phi += spd;
    }
    if (Application::IsKeyPressed('A'))
    {
        theta += spd;
    }
    if (Application::IsKeyPressed('S'))
    {
        phi -= spd;
    }
    if (Application::IsKeyPressed('D'))
    {
        theta -= spd;
    }

    if (Application::IsKeyPressed(VK_UP)) {
        rad -= 20.f * (float)dt;
    }

    if (Application::IsKeyPressed(VK_DOWN)) {
        rad += 20.f * (float)dt;
    } rad = Math::Max(5.f, rad);

    float clampLimit = Math::DegreeToRadian(90.f);
    phi = Math::Clamp(phi, -clampLimit, clampLimit);

    UpdatePos();
}

void Camera::UpdatePos(void) {
    if (theta > Math::PI) 
    {
        theta -= Math::PI;
    }

    if (phi > Math::PI)
    {
        phi -= Math::PI;
    }

    position.x = rad * cos(phi) * cos(theta);
    position.y = rad * sin(phi);
    position.z = rad * cos(phi) * sin(theta);
}
