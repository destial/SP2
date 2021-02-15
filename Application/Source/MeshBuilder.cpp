#include "MeshBuilder.h"
#include <GL\glew.h>
#include "Vertex.h"
#include <vector>
#include "MyMath.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string& meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int index;

	v.pos.Set(-10.0f * lengthX, 0.0f * lengthY, 0.0f * lengthZ); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(10.0f * lengthX, 0.0f * lengthY, 0.0f * lengthZ); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f * lengthX, 0.0f * lengthY, 0.0f * lengthZ); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0.0f * lengthX, -10.0f * lengthY, 0.0f * lengthZ); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f * lengthX, 10.0f * lengthY, 0.0f * lengthZ); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f * lengthX, 0.0f * lengthY, 0.0f * lengthZ); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0.0f * lengthX, 0.0f * lengthY, -10.0f * lengthZ); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f * lengthX, 0.0f * lengthY, 10.0f * lengthZ); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f * lengthX, 0.0f * lengthY, 0.0f * lengthZ); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);


	for (int i = 0; i < 9; i++)
		index_buffer_data.push_back(i);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_LINES;
	mesh->indexSize = 9;

	return mesh;
}

Mesh* MeshBuilder::GenerateSkybox(const std::string& meshName, Color color, float lengthX, float lengthY) {
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

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string& meshName, Color color, float size)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);

	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float s = 1 * size;
	float texturesize = 1;

	v.pos.Set(s, 0, s);
	mesh->corner[Mesh::C1] = Vector3(s, 0, s);
	v.texCoord.Set(texturesize, 0);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-s, 0, s);
	mesh->corner[Mesh::C2] = Vector3(-s, 0, s);
	v.texCoord.Set(0, 0);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-s, 0.f, -s);
	mesh->corner[Mesh::C3] = Vector3(-s, 0, -s);
	v.texCoord.Set(0, texturesize);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(s, 0, s);
	v.texCoord.Set(texturesize, 0);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-s, 0, -s);
	v.texCoord.Set(0, texturesize);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(s, 0, -s);
	mesh->corner[Mesh::C4] = Vector3(s, 0, -s);
	v.texCoord.Set(texturesize, texturesize);
	v.normal = Vector3(0, 1, 0);
	v.color = color;
	vertex_buffer_data.push_back(v);

	for (int i = 0; i < 6; i++) {
		index_buffer_data.push_back(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string& meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
		int index;
		//front
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		mesh->corner[Mesh::C1] = Vector3(0.5f * lengthX, 0, 0.5f * lengthZ);

		//bottom
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		mesh->corner[Mesh::C2] = Vector3(-0.5f * lengthX, 0, -0.5f * lengthZ);

		//right
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		mesh->corner[Mesh::C3] = Vector3(0.5f * lengthX, 0, -0.5f * lengthZ);

		//left
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);

		mesh->corner[Mesh::C2] = Vector3(-0.5f * lengthX, 0, 0.5f * lengthZ);
		//top
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);

		//back
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ); v.color.Set(0, 0, 0); vertex_buffer_data.push_back(v);


	

	for (int i = 0; i < 36; i++)
		index_buffer_data.push_back(i);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = 36;
	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);
	for (unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerSlice = Math::TWO_PI / numSlice;

	v.pos.Set(0, 0, 0); v.color = color;
	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);

	for (int i = 0; i <= numSlice; ++i)
	{
		float theta = i * degreePerSlice;
		v.pos.Set(cos(theta), 0, sin(theta));	v.color = color; v.normal = Vector3(v.pos.x, v.pos.y, v.pos.z).Normalized(); 	vertex_buffer_data.push_back(v);
		v.pos.Set(0, 0, 0);	v.color = color; v.normal.Set(0, 0, 0);	vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < (numSlice + 1) * 2; ++i)
	{
		index_buffer_data.push_back(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, float lengthX, float lengthY)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int index;

	mesh->corner[Mesh::C1] = Vector3(lengthX, 0, lengthY);
	mesh->corner[Mesh::C2] = Vector3(-lengthX, 0, -lengthY);
	mesh->corner[Mesh::C3] = Vector3(lengthX, 0, -lengthY);
	mesh->corner[Mesh::C4] = Vector3(-lengthX, 0, lengthY);
	for (float theta = 0; theta < 360; theta++)
	{
		v.pos.Set(float(cos(theta)), 0, float(sin(theta))); v.color = color; vertex_buffer_data.push_back(v);
		v.pos.Set(0.7 * float(cos(theta)), 0, 0.7 * float(sin(theta))); v.color.Set(0, 1, 1);
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < 720; i++)
		index_buffer_data.push_back(i);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = 720;
	return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float PI = 3.1411592654;

	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);
	for (double phi = 0; phi < 180; phi += 10) {
		for (double theta = 0; theta <= 360; theta += 10) {
			v.pos.Set(radius * cos(phi * PI / 180) * cos(theta * PI / 180),
				radius * sin(phi * PI / 180),
				radius * cos(phi * PI / 180) * sin(theta * PI / 180));
			v.color.Set(1.0f, 1.0f, 0.0f);
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius * cos((phi + 10) * PI / 180) * cos(theta * PI / 180),
				radius * sin((phi + 10) * PI / 180),
				radius * cos((phi + 10) * PI / 180) * sin(theta * PI / 180));
			v.color.Set(1.0f, 0.0f, 0.0f);
			vertex_buffer_data.push_back(v);
		}
	}

	for (int i = 0; i < (10 * 36 * 2 * 2); ++i) {
		index_buffer_data.push_back(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, float height, float radsize, float numstack) {
	Vertex vertex;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int index = 0;

	float radius = 1.0f * radsize;

	float stackheight = height / numstack;

	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);

	for (float stack = 0; stack < numstack; stack++) {
		for (float theta = 0; theta <= 360; theta += 10) {
			float theta_rad = Math::DegreeToRadian(theta);

			vertex.pos.Set(radius * cos(theta_rad), -height / 2 + stack * stackheight, radius * sin(theta_rad));
			vertex.color = color;
			vertex.normal = Vector3(cos(theta_rad), 0, sin(theta_rad));
			vertex_buffer_data.push_back(vertex);
			index_buffer_data.push_back(index++);

			vertex.pos.Set(radius * cos(theta_rad), -height / 2 + (stack + 1) * stackheight, radius * sin(theta_rad));
			vertex.color = color;
			vertex.normal = Vector3(cos(theta_rad), 0, sin(theta_rad));
			vertex_buffer_data.push_back(vertex);
			index_buffer_data.push_back(index++);
		}
	}

	for (float theta = 0; theta <= 360; theta += 10) {
		float theta_rad = Math::DegreeToRadian(theta);

		vertex.pos.Set(radius * cos(theta_rad), height / 2, radius * sin(theta_rad));
		vertex.color = color;
		vertex.normal = Vector3(0, 1, 0);
		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);

		vertex.pos.Set(0, height / 2, 0);
		vertex.color = color;
		vertex.normal = Vector3(0, 1, 0);
		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);
	}

	for (float theta = 0; theta <= 360; theta += 10) {
		float theta_rad = Math::DegreeToRadian(theta);

		vertex.pos.Set(radius * cos(theta_rad), -height / 2, radius * sin(theta_rad));
		vertex.color = color;
		vertex.normal = Vector3(0, -1, 0);
		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);

		vertex.pos.Set(0, -height / 2, 0);
		vertex.color = color;
		vertex.normal = Vector3(0, -1, 0);
		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GeneratehemiSphereWithCircle(const std::string& meshName, Color color, float size)
{
	float radius = size;
	Vertex hemisphere;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);
	for (float phi = 0; phi <= 90; phi += 10) {
		for (float theta = 0; theta <= 360; theta += 10) {
			hemisphere.pos.Set(radius * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), radius * sin(Math::DegreeToRadian(phi)), radius * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));

			hemisphere.normal = Vector3(cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), sin(Math::DegreeToRadian(phi)), cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));

			hemisphere.color = color;
			vertex_buffer_data.push_back(hemisphere);

			hemisphere.pos.Set(radius * cos(Math::DegreeToRadian(phi + 10)) * cos((Math::DegreeToRadian(theta))), radius * sin(Math::DegreeToRadian(phi + 10)), radius * cos(Math::DegreeToRadian(phi + 10)) * sin(Math::DegreeToRadian(theta)));

			hemisphere.normal = Vector3(cos(Math::DegreeToRadian(phi + 10)) * cos((Math::DegreeToRadian(theta))), sin(Math::DegreeToRadian(phi + 10)), cos(Math::DegreeToRadian(phi + 10)) * sin(Math::DegreeToRadian(theta)));

			hemisphere.color = color;
			vertex_buffer_data.push_back(hemisphere);
		}
	}

	for (int i = 0; i < 740; i++) {
		index_buffer_data.push_back(i);
	}

	hemisphere.pos.Set(0, 0, 0);
	hemisphere.normal = Vector3(0, -1, 0);
	vertex_buffer_data.push_back(hemisphere);
	for (float theta = 0; theta <= 360; theta += 10) {
		hemisphere.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		hemisphere.normal = Vector3(0, -1, 0);
		hemisphere.color = color;
		vertex_buffer_data.push_back(hemisphere);
	}
	for (int i = 741; i < 814; i++) {
		index_buffer_data.push_back(740);
		index_buffer_data.push_back(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, float rad, float height, int slice)
{
	Vertex vertex;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int index = 0;

	float radius = 1.0f * rad;

	mesh->corner[Mesh::C1] = Vector3(2 * radius, 0, 2 * radius);
	mesh->corner[Mesh::C2] = Vector3(-2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C3] = Vector3(2 * radius, 0, -2 * radius);
	mesh->corner[Mesh::C4] = Vector3(-2 * radius, 0, 2 * radius);

	for (int theta = 0; theta <= 360; theta += slice) {
		float theta_rad = Math::DegreeToRadian((float)theta);

		Vector3 normal(rad * (cos(theta_rad)), rad, height * sin(float(theta_rad)));
		normal.Normalize();
		vertex.normal = normal;
		vertex.pos.Set(rad * cos(theta_rad), -height / 2, rad * sin(theta_rad));
		vertex.color = color;

		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);

		vertex.pos.Set(0, height / 2, 0);
		vertex.normal = normal;
		vertex.color = color;

		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);
	}

	for (int theta = 0; theta <= 360; theta += slice) {
		float theta_rad = Math::DegreeToRadian((float)theta);

		vertex.normal = Vector3(0, -1, 0);
		vertex.pos.Set(0, -height / 2, 0);
		vertex.color = color;

		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);

		vertex.normal = Vector3(0, -1, 0);
		vertex.pos.Set(rad * cos(theta_rad), -height / 2, rad * sin(theta_rad));
		vertex.color = color;

		vertex_buffer_data.push_back(vertex);
		index_buffer_data.push_back(index++);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, unsigned numStack, unsigned numSlice, float outerR, float innerR, Color colour)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned>index_buffer_data;
	v.color = colour;
	float PI = 3.1411592654;
	float degreePerStack = (180.f / numStack) * (PI / 180);
	float degreePerSlice = (360.f / numSlice) * (PI / 180);
	float x1, z1; //y1 is always zero
	float x2, y2, z2;
	for (unsigned stack = 0; stack < numStack + 1; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			z1 = outerR * cos(stack * degreePerStack);
			x1 = outerR * sin(stack * degreePerStack);
			z2 = (outerR + innerR * cos(slice * degreePerSlice)) * cos(stack * degreePerStack);
			y2 = innerR * sin(slice * degreePerSlice);
			x2 = (outerR + innerR * cos(slice * degreePerSlice)) * sin(stack * degreePerStack);
			v.pos.Set(x2, y2, z2); v.normal.Set((cos(slice * degreePerSlice) * sin(stack * degreePerStack)), (sin(slice * degreePerSlice)), (cos(slice * degreePerSlice) * cos(stack * degreePerStack))); vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);;
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = 1800;
	return mesh;
}

Mesh* MeshBuilder::GeneratePyramid(const std::string& meshName, Color color, float width, float height)
{
	Vertex vertex;
	Mesh* mesh = new Mesh(meshName);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	Vector3 normal;

	mesh->corner[Mesh::C1] = Vector3(width, 0, width);
	mesh->corner[Mesh::C2] = Vector3(-width, 0, -width);
	mesh->corner[Mesh::C3] = Vector3(width, 0, -width);
	mesh->corner[Mesh::C4] = Vector3(-width, 0, width);
	vertex.pos.Set(width, -height / 2, -width);
	normal.Set(vertex.pos.x, 0, vertex.pos.z);
	normal.Normalize();
	vertex.normal = normal;
	vertex.color = color;
	vertex_buffer_data.push_back(vertex); //0: Top right

	vertex.pos.Set(width, -height / 2, width);
	normal.Set(vertex.pos.x, 0, vertex.pos.z);
	normal.Normalize();
	vertex.normal = normal;
	vertex.color = color;
	vertex_buffer_data.push_back(vertex); //1: Bottom right

	vertex.pos.Set(-width, -height / 2, width);
	normal.Set(vertex.pos.x, 0, vertex.pos.z);
	normal.Normalize();
	vertex.normal = normal;
	vertex.color = color;
	vertex_buffer_data.push_back(vertex); //2: Bottom left

	vertex.pos.Set(-width, -height / 2, -width);
	normal.Set(vertex.pos.x, 0, vertex.pos.z);
	normal.Normalize();
	vertex.normal = normal;
	vertex.color = color;
	vertex_buffer_data.push_back(vertex); //3: top left

	vertex.pos.Set(0, height / 2, 0);
	vertex.normal.Set(0, 1, 0);
	vertex.color = color;
	vertex_buffer_data.push_back(vertex); //4: Center at height

	//INDEX BUFFER
	//Bottom right triangle
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	//Top left triangle
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);

	//Triangle in front
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);

	//Triangle at the left
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);

	//Triangle at the back
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);

	//Triangle at right
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size
	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);;

	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0;
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			v.pos.Set(0.5f, 0.5f, 0.f); v.normal.Set(1, 1, 1); v.texCoord.Set(width * (col + 1), -height * (row));    vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, 0.5f, 0.f); v.normal.Set(-1, 1, 1); v.texCoord.Set(width * col, -height * (row));    vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5, 0.f); v.normal.Set(-1, -1, 1); v.texCoord.Set(width * col, -height * (row + 1));    vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0.f); v.normal.Set(1, -1, 1); v.texCoord.Set(width * (col + 1), -height * (row + 1));    vertex_buffer_data.push_back(v);


			//tri1
			index_buffer_data.push_back(0 + offset);
			index_buffer_data.push_back(1 + offset);
			index_buffer_data.push_back(2 + offset);
			//tri2
			index_buffer_data.push_back(0 + offset);
			index_buffer_data.push_back(2 + offset);
			index_buffer_data.push_back(3 + offset);

			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);


	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	std::vector<Material> materials;

	bool success = LoadOBJMTL(file_path.c_str(), mtl_path.c_str(), vertices, uvs, normals, materials);

	if (!success)
		return NULL;

	//Index the vertices, texcoords, & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshName);

	for (Material& material : materials)
		mesh->materials.push_back(material);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_MODE::DRAW_TRIANGLES;
	return mesh;
}


