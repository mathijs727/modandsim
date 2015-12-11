#include "Defines.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "FPSCounter.h"
#include "BoltzmannGridD2Q9.h"
#include "ImageShape.h"

#ifdef OPENMP_ENABLED
#include <omp.h>
#endif
#include <iostream>

void setupOpenGL()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.3f, 0.8f, 0.2f, 1.0f);
}

int main(int argc, char** argv)
{
#ifdef OPENMP_ENABLED
	std::cout << "Number of threads: " << omp_get_max_threads() << std::endl;
#endif
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
	for (int i = 0; i < imageWidth*imageHeight; i++)
	{
		boundaries[i] = BoltzmannGridD2Q9::NoBoundary;
	}
#ifdef _WIN32
	ImageShape object = ImageShape("airfoil.png");
#else
	ImageShape object = ImageShape("../airfoil.png");
#endif
	object.createBoundaries(boundaries, imageWidth, imageHeight, 0, 30);

	BoltzmannGridD2Q9 grid = BoltzmannGridD2Q9(1.0f, imageWidth, imageHeight, boundaries);

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