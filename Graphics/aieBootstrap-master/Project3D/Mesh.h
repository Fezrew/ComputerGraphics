#pragma once
#include "glm/ext.hpp"

using namespace glm;

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void initialiseQuad();

	virtual void draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;

};

