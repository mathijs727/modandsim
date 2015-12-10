#include "Defines.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "FPSCounter.h"
#include "BoltzmannGridD2Q9.h"
#include "ImageShape.h"

#include <iostream>

void setupOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.3f, 0.8f, 0.2f, 1.0f);
}

int main(int argc, char** argv)
{
	Vertex vertices[] = {
		Vertex{ glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		Vertex{ glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		Vertex{ glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f) }
	};

	const int imageWidth = 400;
	const int imageHeight = 200;
	char* boltzmannTexture = new char[imageWidth * imageHeight * 4];
	BoltzmannGridD2Q9::BoundaryType* boundaries = new BoltzmannGridD2Q9::BoundaryType[imageWidth * imageHeight];
	real* initialValues = new real[imageWidth * imageHeight * 9];

	// Initial values
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			boundaries[y * imageWidth + x] = BoltzmannGridD2Q9::NoBoundary;
			for (int i = 0; i < 9; i++) {
				int index = y * imageWidth * 9 + x * 9 + i;
				initialValues[index] = 1. / 14.;
			}
		}
	}

	// Generate circle object in the middle
/*	int diameter = 10;
	int centerWidth = 20;//imageWidth / 2;
	int centerHeight = imageHeight / 2;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			int dx = x - centerWidth;
			int dy = y - centerHeight;
			if (dx*dx + dy*dy < diameter*diameter)
			{
				boundaries[y * imageWidth + x] = BoltzmannGridD2Q9::BounceBackBoundary;
			}
		}
	}*/
	ImageShape object = ImageShape("airfoil.png");
	object.createBoundaries(boundaries, imageWidth, imageHeight, 0, 30);

	BoltzmannGridD2Q9 grid = BoltzmannGridD2Q9(1.0f, imageWidth, imageHeight, initialValues, boundaries);
	delete[] initialValues;

	Window window = Window(800, 400, "Boltzmann fluid simulator");
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
		
		grid.collsionStep();
		grid.streamStep();
		grid.createTexture(boltzmannTexture);

		fpsCounter.update();

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