#include "Application.h"

#include <iostream>

Application::Application() : _windowManager(nullptr), _screenWidth(640), _screenHeight(480) {}

Application::~Application() 
{
	_windowManager = nullptr;
}

void Application::init(int screenWidth, int screenHeight)
{
	// Set screen dimensions
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	// Initialize WindowManager
	_windowManager = WindowManager::getInstance();
	_windowManager->init(_screenWidth, _screenHeight);
	_windowManager->setEventCallbacks(this);

	// Set background color
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void Application::run()
{
	// Get current framebuffer size
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(_windowManager->getHandle(), &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);

	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene...

	// Swap front and back buffers
	glfwSwapBuffers(_windowManager->getHandle());
	
	// Poll for and process events
	glfwPollEvents();
}

bool Application::shouldClose()
{
	return glfwWindowShouldClose(_windowManager->getHandle());
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void Application::resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	_screenWidth = width;
	_screenHeight = height;
}

void Application::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{

}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{

}
