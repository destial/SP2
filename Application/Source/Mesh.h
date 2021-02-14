#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "Material.h"

class Mesh {
public:
	enum DRAW_MODE {
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_TRIANGLE_FAN,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);

	static void SetMaterialLoc(unsigned kA, unsigned kD, unsigned kS, unsigned nS);
	std::vector<Material> materials;
	static unsigned locationKa;
	static unsigned locationKd;
	static unsigned locationKs;
	static unsigned locationNs;

	const std::string name;
	Material material;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;
};

#endif