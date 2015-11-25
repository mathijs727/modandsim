#pragma once
#include <opengl/gl3.h>

class Texture
{
private:
	GLuint m_id;
public:
	Texture(int width, int height, const void* data);
	~Texture();

	void bind() const;
	static void unbind();
};