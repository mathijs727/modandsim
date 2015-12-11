#pragma once
#include <GL/glew.h>

class Texture
{
private:
	GLuint m_id;
public:
	Texture(int width, int height, const void* data);
	~Texture();

	void update(int width, int height, const void* data);

	void bind() const;
	static void unbind();
};