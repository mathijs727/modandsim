#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Vertex* vertices, size_t numVertices)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(
		GL_ARRAY_BUFFER,
		numVertices * sizeof(Vertex),
		vertices,
		GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		false,
		sizeof(Vertex),
		0);

	// Tex coords
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		false,
		sizeof(Vertex),
		(void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void VertexBuffer::bind()
{
	glBindVertexArray(m_vao);
}