#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	static bool s_glfwInitialized;
	GLFWwindow* m_window;
public:
	Window(int width, int height, const char* title);
	~Window();

	bool shouldClose() const;
	void swapBuffers();
	static void pollEvents();
private:
	static void errorCallback(int error, const char* description);
	static void initGlfw();
	static void terminateGlfw();
};

