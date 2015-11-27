#define GLEW_STATIC
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Sequential.cpp"

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
	//char randomTexture[imageWidth * imageHeight * 3];
	char boltzmannTexture[imageWidth * imageHeight * 3];
	real boltzmannGrid[2][imageWidth * imageHeight * 9];

	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageHeight; x++)
		{
			if (x > 20 && x < 150 && y > 50 && y < 150)
			{
				for (int i = 0; i < 9; i++) {
					boltzmannGrid[0][(y * imageWidth + x) * 9 + i] = (real)(1. / 9.);
				}
			} else {
				for (int i = 0; i < 9; i++) {
					boltzmannGrid[0][(y * imageWidth + x) * 9 + i] = 0.0;
				}
			}
		}
	}

	// Fill texture with random values
	/*for (int i = 0; i < imageWidth * imageHeight * 3; i++)
	{
		randomTexture[i] = (char)rand();
	}*/

	Window window = Window(600, 600, "Boltzmann fluid simulator");
	setupOpenGL();

	Texture texture = Texture(imageWidth, imageHeight, boltzmannTexture);
	texture.bind();

	VertexBuffer square = VertexBuffer(vertices, 4);
	square.bind();

	ShaderProgram shader = ShaderProgram("../VertexShader.glsl", "../FragmentShader.glsl");
	shader.bind();

	int grid = 0;
	while (!window.shouldClose())
	{
		Window::pollEvents();

		//collide<real>(boltzmannGrid[grid], imageWidth, imageHeight, 1.0);
		//stream<real>(boltzmannGrid[grid], boltzmannGrid[(grid+1)%2], imageWidth, imageHeight);
		//grid = (grid + 1) % 2;
		//TODO: Boundaries
		createTexture<real>(boltzmannGrid[grid], boltzmannTexture, imageWidth, imageHeight);

		texture.unbind();
		texture.update(imageWidth, imageHeight, boltzmannTexture);
		texture.bind();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		window.swapBuffers();
	}

	return 1;
}