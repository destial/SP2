#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string& meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float size);
	static Mesh* GenerateCube(const std::string& meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateRing(const std::string& meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, float height, float radsize, float numstack);
	static Mesh* GeneratehemiSphereWithCircle(const std::string& meshName, Color color, float size);
	static Mesh* GenerateCone(const std::string& meshName, Color color, float rad, float height, int slice);
	static Mesh* GenerateTorus(const std::string& meshName, unsigned numStack, unsigned numSlice, float outerR, float innerR, Color colour);
	static Mesh* GeneratePyramid(const std::string& meshName, Color color, float width, float height);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
	static Mesh* GenerateSkybox(const std::string& meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateGround(const std::string& meshName, Color color, float size);
};

#endif