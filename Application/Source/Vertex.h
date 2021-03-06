#pragma once
#include <Vector3.h>

struct Position
{
    float x, y, z;
    Position(float x = 0, float y = 0, float z = 0)
    {
        Set(x, y, z);

    }
    void Set(float x, float y, float z)
    {

        this->x = x;
        this->y = y;
        this->z = z;

    }
};

struct Transform {
    Vector3 translate, scale;
    float rotate;
    Transform(Vector3 t = Vector3(0, 0, 0), float s = 1, float r = 0) {
        Set(t, s, r);
    }
    void Set(Vector3 t, float s, float r) {
        this->translate = t;
        this->scale = Vector3(s, s, s);
        this->rotate = r;
    }
    void Translate(float x, float y, float z) {
        this->translate = Vector3(x, y, z);
    }
    void RotateDegree(float degrees) {
        this->rotate = degrees;
    }
    void RotateRadian(float radian) {
        this->rotate = Math::RadianToDegree(radian);
    }
    void Scale(float s) {
        this->scale = Vector3(s, s, s);
    }
    void Scale(float x, float y, float z) {
        this->scale = Vector3(x, y, z);
    }
};

struct TexCoord
{
    float u, v;
    TexCoord(float u = 0, float v = 0) { Set(u, v); }
    void Set(float u, float v) { this->u = u; this->v = v; }
};

struct Color
{

    float r, g, b;
    Color(float r = 0, float g = 0, float b = 0)
    {
        Set(r, g, b);

    }

    void Set(float r = 0, float g = 0, float b = 0)
    {

        this->r = r;
        this->g = g;
        this->b = b;
    }
};

namespace Colors {
	const Color RED = Color(1.f, 0.f, 0.f);
	const Color DARK_RED = Color(.8f, 0.f, 0.f);
	const Color GREEN = Color(0.f, 1.f, 0.f);
	const Color DARK_GREEN = Color(0.f, .8f, 0.f);
	const Color BLUE = Color(0.f, 0.f, 1.f);
	const Color WHITE = Color(1.f, 1.f, 1.f);
	const Color YELLOW = Color(1.f, 1.f, 0.f);
	const Color PURPLE = Color(1.f, 0.f, 1.f);
	const Color CYAN = Color(0.f, 1.f, 1.f);
	const Color BROWN = Color(.6f, .1f, .2f);
	const Color BLACK = Color(0.f, 0.f, 0.f);
	const Color ORANGE = Color((195 / 255), (82 / 255), (20 / 255));
	const Color GRAY = Color(0.4f, 0.4f, 0.4f);
}

    struct Vertex
    {
        Position pos;
        Color color;
        Vector3 normal;
        TexCoord texCoord;
    };


