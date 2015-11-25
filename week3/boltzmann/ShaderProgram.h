#pragma once
#include <opengl/gl3.h>
#include <string>

class ShaderProgram
{
private:
	GLuint m_program;
public:
	ShaderProgram(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
	~ShaderProgram();

	void bind() const;
	void unbind() const;
private:
	std::string readFile(const std::string& filepath);
	GLuint addShader(const std::string& text, GLenum type);
	void compileProgram();
};