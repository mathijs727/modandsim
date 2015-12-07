#define GLEW_STATIC
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "FPSCounter.h"
#include "BoltzmannGridD2Q9.h"

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

	const int imageWidth = 50;
	const int imageHeight = 50;
	char* boltzmannTexture = new char[imageWidth * imageHeight * 4];
	BoltzmannGridD2Q9::BoundaryType* boundaries = new BoltzmannGridD2Q9::BoundaryType[imageWidth * imageHeight];
	real* initialValues = new real[imageWidth * imageHeight * 9];
	
	// Initial values
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			boundaries[y * imageWidth + x] = BoltzmannGridD2Q9::NoBoundary;

			/*if (x > 10 && x < 40 && y > 10 && y < 40)
			{
				for (int i = 0; i < 9; i++) {
					int index = y * imageWidth * 9 + x * 9 + i;
					initialValues[index] = 1. / 9.;
				}
			} else {*/
				for (int i = 0; i < 9; i++) {
					int index = y * imageWidth * 9 + x * 9 + i;
					initialValues[index] = 1. / 14.;
				}
			//}
		}
	}

	// Create boundary at edge of grid
	/*for (int y = 0; y < imageHeight; y++) {
		boundaries[y * imageWidth + 0] = BoltzmannGridD2Q9::BounceBackBoundary;
		boundaries[y * imageWidth + imageWidth - 1] = BoltzmannGridD2Q9::BounceBackBoundary;
	}
	*for (int x = 0; x < imageWidth; x++)
	{
		boundaries[x] = BoltzmannGridD2Q9::BounceBackBoundary;
		boundaries[(imageHeight-1) * imageWidth + x] = BoltzmannGridD2Q9::BounceBackBoundary;
	}*/

	// Generate circle object in the middle
	int diameter = 10;
	int centerWidth = imageWidth / 2;
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
	}

	// Create generator and drain
	for (int y = 1; y < imageHeight-1; y++) {
		//boundaries[y * imageWidth + 1] = BoltzmannGridD2Q9::Generator;
		//boundaries[y * imageWidth + imageWidth-2] = BoltzmannGridD2Q9::Drain;
	}


	BoltzmannGridD2Q9 grid = BoltzmannGridD2Q9(1.0f, imageWidth, imageHeight, initialValues, boundaries);
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