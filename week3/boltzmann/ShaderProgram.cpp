#include "ShaderProgram.h"

#include <fstream>
#include <vector>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath)
{
	m_program = glCreateProgram();

	if (m_program == 0)
	{
		std::cout << "Shader creation failed: Could not find valid memory location \
			in constructor of ShaderProgram" << std::endl;
	}

	GLuint vertexShader = addShader(readFile(vertexShaderFilepath).c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = addShader(readFile(fragmentShaderFilepath).c_str(), GL_FRAGMENT_SHADER);

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	compileProgram();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

void ShaderProgram::bind() const
{
	glUseProgram(m_program);
}

void ShaderProgram::unbind() const
{
	glUseProgram(0);
}

std::string ShaderProgram::readFile(const std::string& filepath)
{
	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cout << "Could not open shader file: " << filepath << std::endl;
		return std::string("");
	}

	std::string contents((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file.close();

	return contents;
}

GLuint ShaderProgram::addShader(const std::string& text, GLenum type)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0)
	{
		std::cout << "Shader creation failed, probably caused by lack of GPU memory" << std::endl;
		exit(1);
	}

	const char* source = text.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(shader, length, &length, &error[0]);

		std::cout << "Failed to compile shader: " << &error[0] << std::endl;
		glDeleteShader(shader);
	}

	return shader;
}

void ShaderProgram::compileProgram()
{
	GLint result;

	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length+1);
		glGetProgramInfoLog(m_program, length, &length, &error[0]);

		std::cout << "Failed to link shader program: " << &error[0] << std::endl;

		glDeleteShader(m_program);
	}

	glValidateProgram(m_program);
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length+1);
		glGetProgramInfoLog(m_program, length, &length, &error[0]);

		std::cout << "Failed to validate shader program: " << &error[0] << std::endl;

		glDeleteShader(m_program);
	}
}