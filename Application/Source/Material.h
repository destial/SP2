#pragma once
struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b)
	{
		this->r = r; this->g = g; this->b = b;
	}
    Component& operator=(const Component& rhs)
    {
        Set(rhs.r, rhs.g, rhs.b);
    }
};
struct Material
{
    Component kAmbient;
    Component kDiffuse;
    Component kSpecular;
    float kAlpha;
    float kShininess;
    int size;
    Material() {
        kAmbient.Set(0.1f, 0.1f, 0.1f);
        kDiffuse.Set(0.3f, 0.3f, 0.3f);
        kSpecular.Set(0.6f, 0.6f, 0.6f);
        kAlpha = 1.f;
        kShininess = 1.f;
        size = 0;
    }
    Material& operator=(const Material& rhs)
    {
        kAmbient = rhs.kAmbient;
        kDiffuse = rhs.kDiffuse;
        kSpecular = rhs.kSpecular;
        kAlpha = rhs.kAlpha;
        kShininess = rhs.kShininess;
        size = rhs.size;
    }
};