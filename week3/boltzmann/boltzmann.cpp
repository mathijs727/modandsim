#define GLEW_STATIC
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "FPSCounter.h"
#include "BoltzmannGridD2Q9.h"

#include <chrono>
#include <iostream>

void setupOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.3f, 0.8f, 0.2f, 1.0f);
}

typedef float real;

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
	char* boltzmannTexture = new char[imageWidth * imageHeight * 3];
	BoltzmannGrid2D9Q::BoundaryTypes* boundaries = new BoltzmannGrid2D9Q::BoundaryTypes[imageWidth * imageHeight];
	real* initialValues = new real[imageWidth * imageHeight * 9];
	real tau = 1.0;

	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageHeight; x++)
		{
			if (x > 20 && x < 150 && y > 50 && y < 150)
			{
				for (int i = 0; i < 9; i++) {
					int index = i * (imageWidth * imageHeight) + y * imageWidth + x;
					initialValues[index] = 1. / 9.;
				}
			} else {
				for (int i = 0; i < 9; i++) {
					int index = i * (imageWidth * imageHeight) + y * imageWidth + x;
					initialValues[index] = 0.0;
				}
			}
		}
	}
	BoltzmannGrid2D9Q grid = BoltzmannGrid2D9Q(1.0f, imageWidth, imageHeight, initialValues, boundaries);
	delete[] initialValues;

	Window window = Window(600, 600, "Boltzmann fluid simulator");
	setupOpenGL();

	Texture texture = Texture(imageWidth, imageHeight, boltzmannTexture);
	texture.bind();

	VertexBuffer square = VertexBuffer(vertices, 4);
	square.bind();

#ifdef _WIN32
	ShaderProgram shader = ShaderProgram("VertexShader.glsl", "FragmentShader.glsl");
#else
	ShaderProgram shader = ShaderProgram("../VertexShader.glsl", "../FragmentShader.glsl");
#endif
	shader.bind();

	FPSCounter fpsCounter = FPSCounter();
	while (!window.shouldClose())
	{
		Window::pollEvents();
		fpsCounter.update();

		//TODO: Boundaries
		grid.collsionStep();
		grid.streamStep();
		grid.createTexture(boltzmannTexture);

		texture.unbind();
		texture.update(imageWidth, imageHeight, boltzmannTexture);
		texture.bind();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		window.swapBuffers();
	}

	delete[] boltzmannTexture;
	delete[] boundaries;

	return 1;
}