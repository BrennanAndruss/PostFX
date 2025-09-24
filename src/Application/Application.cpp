#include "Application.h"

#include <iostream>
#include <memory>

#include "Time.h"
#include "../Renderer/ForwardRenderPass.h"

Application::Application() : _windowManager(nullptr), _fbWidth(640), _fbHeight(480), 
	_mouseTrapped(false), _firstMouse(false), _translateInput(), _rotateInput() {}

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
	_resourceManager->loadShader("texture", "shaders/texture.vert", "shaders/texture.frag");

	size_t texID = _resourceManager->loadTexture("texture", "textures/labonetex.jpg", false);

	size_t cubeID = _resourceManager->loadMesh("cube", "models/cube.obj");
	std::shared_ptr<Mesh> cubeMesh = _resourceManager->getMesh(cubeID);
	size_t planeID = _resourceManager->loadMesh("plane", "models/plane.obj");
	std::shared_ptr<Mesh> planeMesh = _resourceManager->getMesh(planeID);
	size_t armadilloID = _resourceManager->loadMesh("armadillo", "models/Armadillo.obj");
	std::shared_ptr<Mesh> armadilloMesh = _resourceManager->getMesh(armadilloID);

	size_t matID = _resourceManager->loadMaterial("simple", "simple");
	std::shared_ptr<Material> material = _resourceManager->getMaterial(matID);
	material->ambient = glm::vec3(0.2f, 0.0f, 0.0f);
	material->diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
	material->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	material->shininess = 32.0f;

	size_t texMatID = _resourceManager->loadMaterial("texture", "texture");
	std::shared_ptr<Material> texMaterial = _resourceManager->getMaterial(texMatID);
	texMaterial->texture = _resourceManager->getTexture("texture");

	// Initialize the scene
	float aspect = static_cast<float>(_fbWidth) / static_cast<float>(_fbHeight);
	_camera = std::make_shared<Camera>(glm::vec3(-5.0f, 0.0f, 0.0f), aspect);
	_scene.setCamera(_camera);

	auto plane = std::make_shared<Object>(planeMesh, texMaterial);
	plane->transform.translation = glm::vec3(0.0f, -1.0f, 0.0f);
	plane->transform.scale = glm::vec3(10.0f, 1.0f, 10.0f);
	_scene.addObject(plane);
	
	auto armadillo = std::make_shared<Object>(armadilloMesh, material);
	armadillo->transform.scale = glm::vec3(0.025f);
	_scene.addObject(armadillo);

	auto cube2 = std::make_shared<Object>(cubeMesh, material);
	cube2->transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	_scene.addObject(cube2);

	// Initialize render pipeline
	_renderPipeline.init(_scene, _fbWidth, _fbHeight);
	_renderPipeline.addRenderPass(std::make_unique<ForwardRenderPass>());

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Application::run()
{
	// Update camera
	_camera->updatePosition(_translateInput, Time::deltaTime());
	_camera->updateRotation(_rotateInput);

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
		// Release the cursor to leave the window
		if (_mouseTrapped)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			_mouseTrapped = false;
			_firstMouse = true;
		}
		else
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	// Camera movement inputs
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS) _translateInput.z += 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.z -= 1.0f;
	}
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS) _translateInput.x -= 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.x += 1.0f;
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS) _translateInput.z -= 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.z += 1.0f;
	}
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS) _translateInput.x += 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.x -= 1.0f;
	}
	if (key == GLFW_KEY_E)
	{
		if (action == GLFW_PRESS) _translateInput.y += 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.y -= 1.0f;
	}
	if (key == GLFW_KEY_Q)
	{
		if (action == GLFW_PRESS) _translateInput.y -= 1.0f;
		else if (action == GLFW_RELEASE) _translateInput.y += 1.0f;
	}
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Capture the cursor to enable free rotation
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		_mouseTrapped = true;
	}
}

void Application::resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (width != _fbWidth || height != _fbHeight)
	{
		_fbWidth = width;
		_fbHeight = height;
	}

	_camera->updateAspect(static_cast<float>(_fbWidth) / static_cast<float>(_fbHeight));
	_renderPipeline.resize(_fbWidth, _fbHeight);
}

void Application::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	static double xPrev = 0.0;
	static double yPrev = 0.0;
	static float sensitivity = 0.01f;

	if (!_mouseTrapped) 
		return;

	if (_firstMouse)
	{
		xPrev = xPos;
		yPrev = yPos;
		_firstMouse = false;
	}

	// Calculate deltas
	double xOffset = xPos - xPrev;
	double yOffset = yPos - yPrev;
	xPrev = xPos;
	yPrev = yPos;

	// Flip vertical rotation to follow mouse
	_rotateInput.y = -static_cast<float>(yOffset) * sensitivity;
	_rotateInput.x = static_cast<float>(xOffset) * sensitivity;
}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	
}