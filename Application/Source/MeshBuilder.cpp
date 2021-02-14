#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vector3.h"
#include "MyMath.h"
#include "LoadOBJ.h"

Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ) {
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;
	v.color.Set(1.f, 0, 0);
	v.pos.Set(-lengthX, 0.f, 0.f); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0.f, 0.f); vertex_buffer_data.push_back(v);

	v.color.Set(0, 1.f, 0);
	v.pos.Set(0.f, -lengthY, 0.f); vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, lengthY, 0.f); vertex_buffer_data.push_back(v);

	v.color.Set(0, 0, 1.f);
	v.pos.Set(0.f, 0.f, -lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, 0.f, lengthZ); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 6; i++) {
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;
	return mesh;
}

Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = color;
	v.normal.Set(0, 0, 1);

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f); 
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f); 
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateGround(const std::string& meshName, Color color, float lengthX, float lengthY) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = color;
	v.normal.Set(0, 0, 1);

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f);
	v.texCoord.Set(1000, 1000);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f);
	v.texCoord.Set(0, 1000);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f);
	v.texCoord.Set(1000, 0);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = color;
	v.texCoord.Set(0, 0);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);

	v.texCoord.Set(1, 0);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);

	v.texCoord.Set(1, 1);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);

	v.texCoord.Set(0, 1);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);

	v.texCoord.Set(0, 0);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);

	v.texCoord.Set(0, 1);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 36; ++i) {
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

float sphereX(double& phi, double& theta) {
	return (cos(phi) * cos(theta));
}

float sphereY(double& phi, double& theta) {
	return (sin(phi));
}

float sphereZ(double& phi, double& theta) {
	return (cos(phi) * sin(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius) {
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	unsigned index = 0;
	double pi = Math::PI;
	double degreePerStack = pi / numStack;
	double degreePerSlice = (2 * pi) / numSlice;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		double phi = -(pi / 2) + stack * degreePerStack;

		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			double theta = slice * degreePerSlice;

			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));

			v.texCoord.Set(slice, stack);

			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			v.normal.Normalize();
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice );
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float innerRadius, float outerRadius) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = Math::TWO_PI / numStack;
	float degreePerSlice = Math::TWO_PI / numSlice;
	float x1, z1;
	float x2, y2, z2;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			z1 = outerRadius * cos(stack * degreePerStack);
			x1 = outerRadius * sin(stack * degreePerStack);
			z2 = (outerRadius + innerRadius * cos(slice * degreePerSlice)) * cos(stack * degreePerStack);
			y2 = innerRadius * sin(slice * degreePerSlice);
			x2 = (outerRadius + innerRadius * cos(slice * degreePerSlice)) * sin(stack * degreePerStack);
			v.pos.Set(x2, y2, z2); v.color = color; vertex_buffer_data.push_back(v);
			v.pos.Set(x2-x1, y2, z2-z1); v.color = color; vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned numSlice, float radius, float height) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degree = Math::TWO_PI / numSlice;

	// bottom circle
	v.pos.Set(0, -0.5f * height, 0);
	v.normal.Set(0, -1, 0);
	//v.texCoord.Set(0.5f, 0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degree;
		v.pos.Set(radius * cos(theta), -0.5f * height, radius * sin(theta));
		v.normal.Set(0, -1, 0);
		//v.texCoord.Set(slice / numSlice, slice / numSlice);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(slice + 1);
	}

	// curve
	v.texCoord.Set(0.5f, 0.5f);
	unsigned cylinderStartIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degree;
		// bottom
		v.pos.Set(radius * cos(theta), -0.5f * height, radius * sin(theta));
		v.normal.Set(cos(theta), 0, sin(theta));
		//v.texCoord.Set(slice / numSlice, slice / numSlice);
		vertex_buffer_data.push_back(v);

		// top
		v.pos.Set(radius * cos(theta), 0.5f * height, radius * sin(theta));
		//v.texCoord.Set(slice / numSlice, slice / numSlice);
		v.normal.Set(cos(theta), 0, sin(theta));
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(cylinderStartIndex + 2 * slice + 0);
		index_buffer_data.push_back(cylinderStartIndex + 2 * slice + 1);
	}

	// top circle
	unsigned circleStartIndex = vertex_buffer_data.size();
	v.pos.Set(0, 0.5f * height, 0);
	v.normal.Set(0, 1, 0);
	//v.texCoord.Set(0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degree;
		v.pos.Set(radius * cos(theta), 0.5f * height, radius * sin(theta));
		v.normal.Set(0, 1, 0);
		v.texCoord.Set(slice / numSlice, slice / numSlice);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(circleStartIndex + slice + 1);
		index_buffer_data.push_back(circleStartIndex + 0);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float radius, float height) {
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degree = 360.f / numSlice;
	unsigned circleStartIndex = vertex_buffer_data.size();
	v.pos.Set(0, -0.5f * height, 0);
	v.normal.Set(0, -1, 0);
	//v.texCoord.Set(0.5f, 0.5f);
	vertex_buffer_data.push_back(v);

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degree;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(0, -1, 0);
		//v.texCoord.Set(slice / numSlice, slice / numSlice);
		v.color = color;
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(circleStartIndex);
		index_buffer_data.push_back(circleStartIndex + slice + 1);
	}

	unsigned coneStartIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		float theta = slice * degree;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -0.5f * height, radius * sin(Math::DegreeToRadian(theta)));
		v.normal.Set(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		v.normal.Normalize();
		//v.texCoord.Set(slice / numSlice, slice / numSlice);
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0.5f * height, 0);
		//v.texCoord.Set(0.5f, 0.5f);
		v.normal.Set(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		v.normal.Normalize();
		v.color = color;
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
		index_buffer_data.push_back(coneStartIndex + 2 * slice + 0);
		index_buffer_data.push_back(coneStartIndex + 2 * slice + 1);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path) {
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;

	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success) return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& obj_path, const std::string& mtl_path) {
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	std::vector<Material> materials;
	bool success = LoadOBJMTL(obj_path.c_str(), mtl_path.c_str(), vertices, uvs, normals, materials);
	if (!success) return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshName);
	for (Material& material : materials) {
		mesh->materials.push_back(material);
	}
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.normal.Set(0, 0, 1);

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0.f;
	for (float row = (numRow-1); row >= 0; --row) {
		for (unsigned col = 0; col < numCol; ++col) {
			v.pos.Set(-0.5, -0.5, 0); 
			v.texCoord.Set(width * (col), height * (row));
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, -0.5, 0);
			v.texCoord.Set(width * (col + 1), height * (row));
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, 0.5, 0); 
			v.texCoord.Set(width * (col + 1), height * (row + 1));
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0); 
			v.texCoord.Set(width * (col), height * (row + 1));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);

			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius) {
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degree = Math::TWO_PI / numSlice;
	v.pos.Set(0, 0, 0);
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	index_buffer_data.push_back(0);
	for (unsigned i = 0; i < numSlice + 1; ++i) {
		float theta = i * degree;
		v.pos.Set(radius*cos(theta), 0, radius*sin(theta));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned i = 0; i < numSlice + 1; ++i) {
		index_buffer_data.push_back(i + 1);
		index_buffer_data.push_back(0);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateQuarterTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	v.color = color;

	float degreePerStack = Math::HALF_PI / numStack;
	float degreePerSlice = Math::TWO_PI / numSlice;
	float x1, z1, x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = stack * degreePerStack;
		z1 = outerR * cos(phi);
		x1 = outerR * sin(phi);
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			z2 = (outerR + innerR * cos(theta)) * cos(phi);
			y2 = innerR * sin(theta);
			x2 = (outerR + innerR * cos(theta)) * sin(phi);
			v.pos.Set(x2, y2, z2);
			v.normal.Set(x2 - x1, y2, z2 - x1);
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++ slice) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}