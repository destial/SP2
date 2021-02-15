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

    struct Vertex
    {
        Position pos;
        Color color;
        Vector3 normal;
        TexCoord texCoord;
    };


