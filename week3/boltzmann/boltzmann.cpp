#define GLEW_STATIC
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"

#include <stdlib.h>
#include <iostream>
#include <vector>

void setupOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.3f, 0.8f, 0.2f, 1.0f);
}

int main(int argc, char** argv)
{
	Vertex vertices[] = {
		Vertex{ glm::vec2(-1.0f, 1.0f), glm::vec2(-1.0f, 1.0f) },
		Vertex{ glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec2(-1.0f, -1.0f), glm::vec2(-1.0f, -1.0f) },
		Vertex{ glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, -1.0f) }
	};

	const int imageWidth = 200;
	const int imageHeight = 200;
	char randomTexture[imageWidth * imageHeight * 3];
	
	// Fill texture with random values
	for (int i = 0; i < imageWidth * imageHeight * 3; i++)
	{
		randomTexture[i] = rand();
	}

	Window window = Window(800, 800, "Boltzmann fluid simulator");
	setupOpenGL();

	Texture texture = Texture(imageWidth, imageHeight, randomTexture);
	texture.bind();

	VertexBuffer square = VertexBuffer(vertices, 4);
	square.bind();

	ShaderProgram shader = ShaderProgram("VertexShader.glsl", "FragmentShader.glsl");
	shader.bind();


	while (!window.shouldClose())
	{
		Window::pollEvents();
		
		//TODO: run simulation and display it using OpenGL
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		window.swapBuffers();
	}
	std::cout << "Hello world!" << std::endl;

	return 1;
}