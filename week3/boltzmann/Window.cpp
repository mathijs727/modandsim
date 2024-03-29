#include "Window.h"
#include "ErrorHandling.h"
#include <iostream>

bool Window::s_glfwInitialized = false;

Window::Window(int width, int height, const char* title)
{
	/* Initialize the GLFW library */
	initGlfw();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(errorCallback);

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_window)
	{
		terminateGlfw();
		error::crash("Could not create GLFW window.");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;// To get OpenGL 3+ features working on OS X
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Error initializing glew:\n" << glewGetErrorString(err) << std::endl;
	}

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	terminateGlfw();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}


void Window::errorCallback(int error, const char* description)
{
	std::cout << "GLFW Error(" << error << "):\n" << description << std::endl;
}

void Window::initGlfw()
{
	if (!s_glfwInitialized)
	{
		if (!glfwInit())
		{
			error::crash("Could not initialize GLFW.");
		}

		s_glfwInitialized = true;
	}
}

void Window::terminateGlfw()
{
	if (s_glfwInitialized)
	{
		glfwTerminate();
		s_glfwInitialized = false;
	}
}