#include "MatrixStack.h"

MS::MS() {
	Mtx44 mat;
	mat.SetToIdentity();
	ms.push(mat);
}

MS::~MS() {
}

const Mtx44& MS::Top() const {
	return ms.top();
}

void MS::PopMatrix() {
	ms.pop();
}

void MS::PushMatrix() {
	ms.push(ms.top());
}

void MS::Clear() {
	while(ms.size() > 1)
		ms.pop();
}

void MS::LoadIdentity() {
	Mtx44 mat;
	mat.SetToIdentity();
	ms.top() = mat;
}

void MS::LoadMatrix(const Mtx44 &matrix) {
	ms.top() = matrix;
}

void MS::MultMatrix(const Mtx44 &matrix) {
	ms.top() = ms.top() * matrix;
}

void MS::Rotate(float degrees, float axisX, float axisY, float axisZ) {
	Mtx44 mat;
	mat.SetToRotation(degrees, axisX, axisY, axisZ);
	ms.top() = ms.top() * mat;
}

void MS::Scale(float scaleX, float scaleY, float scaleZ) {
	Mtx44 mat;
	mat.SetToScale(scaleX, scaleY, scaleZ);
	ms.top() = ms.top() * mat;
}

void MS::Translate(float translateX, float translateY, float translateZ) {
	Mtx44 mat;
	mat.SetToTranslation(translateX, translateY, translateZ);
	ms.top() = ms.top() * mat;
}

void MS::Frustum(double left, double right, double bottom, double top, double near, double far) {
	Mtx44 mat;
	mat.SetToFrustum(left, right, bottom, top, near, far);
	ms.top() = ms.top() * mat;
}

void MS::LookAt(double eyeX, double eyeY, double eyeZ,
				double centerX, double centerY, double centerZ,
				double upX, double upY, double upZ) {
	Mtx44 mat;
	mat.SetToLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	ms.top() = ms.top() * mat;
}
