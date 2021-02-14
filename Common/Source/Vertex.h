#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

struct Position {
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0) {
		Set(x, y, z);
	}
	void Set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Color {
	float r, g, b;
	Color(float r = 1, float g = 1, float b = 1) {
		Set(r, g, b);
	}
	void Set(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

struct Transform {
	float rotate;
	Vector3 translate;
	float scale;
	Vector3 aScale;
	Transform(float r = 0, float tX = 0, float tY = 0, float tZ = 0, float s = 1, float sX = 1, float sY = 1, float sZ = 1) {
		Set(r, tX, tY, tZ, s, sX, sY, sZ);
	}
	void Set(float r, float tX, float tY, float tZ, float s, float sX, float sY, float sZ) {
		this->rotate = r;
		this->translate.x = tX;
		this->translate.y = tY;
		this->translate.z = tZ;
		this->scale = s;
		this->aScale.x = sX;
		this->aScale.y = sY;
		this->aScale.z = sZ;
		if (scale != 1) {
			this->aScale.x = scale;
			this->aScale.y = scale;
			this->aScale.z = scale;
		}
	}
	void Set(Transform& t) {
		this->rotate = t.rotate;
		this->translate.x = t.translate.x;
		this->translate.y = t.translate.y;
		this->translate.z = t.translate.z;
		this->scale = t.scale;
		this->aScale = t.aScale;
		if (t.scale != 1) {
			this->aScale.x = t.scale;
			this->aScale.y = t.scale;
			this->aScale.z = t.scale;
		}
	}
};

struct TexCoord {
	float u, v;
	TexCoord(float u = 0, float v = 0) {
		Set(u, v);
	}
	void Set(float u, float v) {
		this->u = u;
		this->v = v;
	}
};

struct Vertex {
	Position pos;
	Color color;
	Vector3 normal;
	TexCoord texCoord;
};

#endif