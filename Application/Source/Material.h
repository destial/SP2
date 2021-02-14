#ifndef MATERIAL_H
#define MATERIAL_H

struct Component {
	float r, g, b;
	Component(float r = 1.f, float g = 1.f, float b = 1.f) {
		Set(r, g, b);
	}
	Component& operator=(const Component& rhs) {
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		return *this;
	}
	void Set(float r, float g, float b) {
		this->r = r; this->g = g; this->b = b;
	}
};

struct Material {
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;
	unsigned size;
	Material() {
		this->kAmbient.Set(1.f, 1.f, 1.f);
		this->kAmbient.Set(1.f, 1.f, 1.f);
		this->kSpecular.Set(1.f, 1.f, 1.f);
		this->kShininess = 1.f;
		this->size = 0;
	}
	Material& operator=(const Material& rhs) {
		kAmbient = rhs.kAmbient;
		kDiffuse = rhs.kDiffuse;
		kSpecular = rhs.kSpecular;
		kShininess = rhs.kShininess;
		size = rhs.size;
		return *this;
	}
};

#endif