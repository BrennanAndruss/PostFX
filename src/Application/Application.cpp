#include "Application.h"

#include <iostream>
#include <memory>

#include "Time.h"
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

	// Initialize OpenGL state
	glEnable(GL_DEPTH_TEST);

	// Initialize resources
	_resourceManager = ResourceManager::getInstance();
	_resourceManager->setResourceDir("../../../resources/");
	_resourceManager->loadShader("simple", "shaders/simple.vert", "shaders/simple.frag");

	size_t cubeID = _resourceManager->loadMesh("cube", "models/cube.obj");
	std::shared_ptr<Mesh> cubeMesh = _resourceManager->getMesh(cubeID);

	size_t matID = _resourceManager->loadMaterial("simple", "simple");
	std::shared_ptr<Material> material = _resourceManager->getMaterial(matID);
	material->ambient = glm::vec3(1.0f, 0.0f, 0.0f);

	// Initialize the scene
	float aspect = static_cast<float>(_fbWidth) / static_cast<float>(_fbHeight);
	auto camera = std::make_shared<Camera>(glm::vec3(-5.0f, 0.0f, 0.0f), aspect);
	_scene.setCamera(camera);
	
	auto cube = std::make_shared<Object>(cubeMesh, material);
	// cube->transform.translation = glm::vec3(0.0f, 0.0f, 500.0f);
	_scene.addObject(cube);

	// Initialize render pipeline
	_renderPipeline.init(_scene, _fbWidth, _fbHeight);
	_renderPipeline.addRenderPass(std::make_unique<ForwardRenderPass>());

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Application::run()
{
	// Update camera

	// Render scene
	_renderPipeline.render(_scene);

	// Swap front and back buffers
	glfwSwapBuffers(_windowManager->getHandle());
	
	// Poll for and process events
	glfwPollEvents();
	Time::update();
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
