#pragma once
#include "Vertex.h"

#include <GL/glew.h>
#include <vector>

class VertexBuffer
{
private:
	GLuint m_vao, m_vbo;
public:
	VertexBuffer(const Vertex* vertices, size_t numVertices);
	~VertexBuffer();

	void bind();
};