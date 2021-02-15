#ifndef LIGHT_H
#define LIGHT_H
#include "Vertex.h"
struct Light
{
    enum LIGHT_TYPE
    {
        LIGHT_POINT = 0,
        LIGHT_DIRECTIONAL,
        LIGHT_SPOT,
    };
    LIGHT_TYPE type;
    Vector3 spotDirection;
    float cosCutoff;
    float cosInner;
    float exponent;

    Position position;
    Color color;
    float power;
    float kC, kL, kQ;
    Light() {
        power = 1.f;
        kC = 1.0f;
        kL = 0.01f;
        kQ = 0.001;
    }
};
#endif