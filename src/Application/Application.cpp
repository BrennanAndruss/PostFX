#include "Application.h"

#include <iostream>
#include <memory>

#include "../Renderer/ForwardRenderPass.h"

Application::Application() : _windowManager(nullptr), _fbWidth(640), _fbHeight(480) {}

Application::~Application() 
{
	_windowManager = nullptr;
}

void Application::init(int screenWidth, int screenHeight)
{
	// Set screen dimensions
	_fbWidth = screenWidth;
	_fbHeight = screenHeight;

	// Initialize WindowManager
	_windowManager = WindowManager::getInstance();
	_windowManager->init(_fbWidth, _fbHeight);
	_windowManager->setEventCallbacks(this);

	// Initialize resources
	_resourceManager = ResourceManager::getInstance();
	_resourceManager->setResourceDir("../../../resources/");
	_resourceManager->loadShader("simple", "shaders/simple.vert", "shaders/simple.frag");

	// Initialize render pipeline
	_renderPipeline.addRenderPass(std::make_unique<ForwardRenderPass>());

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Application::run()
{
	// Render scene
	_renderPipeline.render();

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
	if (width != _fbWidth || height != _fbHeight)
	{
		_fbWidth = width;
		_fbHeight = height;
	}
}

void Application::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{

}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{

}
