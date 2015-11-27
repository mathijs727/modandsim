#define GLEW_STATIC
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "FPSCounter.h"
#include "BoltzmannGridD2Q9.h"

#include <chrono>
#include <iostream>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

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
		Vertex{ glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		Vertex{ glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		Vertex{ glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f) }
	};

	const int imageWidth = 50;
	const int imageHeight = 50;
	char* boltzmannTexture = new char[imageWidth * imageHeight * 4];
	BoltzmannGrid2D9Q::BoundaryType* boundaries = new BoltzmannGrid2D9Q::BoundaryType[imageWidth * imageHeight];
	real* initialValues = new real[imageWidth * imageHeight * 9];
	real tau = 1.0;
	
	// Initial values
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			boundaries[y * imageWidth + x] = BoltzmannGrid2D9Q::NoBoundary;
			if (x > 10 && x < imageWidth-10 && y > 10 && y < imageHeight-10)
			{
				for (int i = 0; i < 9; i++) {
					int index = i * (imageWidth * imageHeight) + y * imageWidth + x;
					initialValues[index] = 1. / 10.;
				}
			} else {
				for (int i = 0; i < 9; i++) {
					int index = i * (imageWidth * imageHeight) + y * imageWidth + x;
					initialValues[index] = 0.0;
				}
			}
		}
	}

	// Create boundary at edge of grid
	for (int y = 0; y < imageHeight; y++)
	{
		boundaries[y * imageWidth + 0] = BoltzmannGrid2D9Q::BounceBackBoundary;
		boundaries[y * imageWidth + imageWidth-1] = BoltzmannGrid2D9Q::BounceBackBoundary;
	}
	for (int x = 0; x < imageWidth; x++)
	{
		boundaries[x] = BoltzmannGrid2D9Q::BounceBackBoundary;
		boundaries[(imageHeight-1) * imageWidth + x] = BoltzmannGrid2D9Q::BounceBackBoundary;
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
		int milliseconds = 80;
#ifdef WIN32
		Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
		struct timespec ts;
    	ts.tv_sec = milliseconds / 1000;
    	ts.tv_nsec = (milliseconds % 1000) * 1000000;
    	nanosleep(&ts, NULL);
#else
		usleep(milliseconds * 1000);
#endif
		Window::pollEvents();
		fpsCounter.update();

		//TODO: Boundaries
		//grid.collsionStep();
		grid.streamStep();
		grid.boundaryStep();
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