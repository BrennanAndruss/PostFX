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
	glEnable(GL_CULL_FACE);

	// Initialize resources
	_resourceManager = ResourceManager::getInstance();
	_resourceManager->setResourceDir("../../../resources/");

	size_t simpleID = _resourceManager->loadShader("simple", "shaders/simple.vert", "shaders/simple.frag");
	std::shared_ptr<Shader> simpleShader = _resourceManager->getShader(simpleID);
	_resourceManager->loadShader("texture", "shaders/texture.vert", "shaders/texture.frag");
	_resourceManager->loadShader("cel", "shaders/cel.vert", "shaders/cel.frag");
	_resourceManager->loadShader("outline", "shaders/outline.vert", "shaders/outline.frag");
	size_t outlineID = _resourceManager->loadShader("normal", "shaders/normals.vert", "shaders/normals.frag", "shaders/normals.geom");
	std::shared_ptr<Shader> outlineShader = _resourceManager->getShader(outlineID);

	size_t texID = _resourceManager->loadTexture("texture", "textures/labonetex.jpg", false);

	std::cout << "Loading models...\n";
	size_t meshID = _resourceManager->loadMesh("cube", "models/cube.obj");
	std::shared_ptr<Mesh> cubeMesh = _resourceManager->getMesh(meshID);
	meshID = _resourceManager->loadMesh("plane", "models/plane.obj");
	std::shared_ptr<Mesh> planeMesh = _resourceManager->getMesh(meshID);
	meshID = _resourceManager->loadMesh("armadillo", "models/Armadillo.obj");
	std::shared_ptr<Mesh> armadilloMesh = _resourceManager->getMesh(meshID);
	meshID = _resourceManager->loadMesh("dragon", "models/dragon_vrip_res3.obj");
	std::shared_ptr<Mesh> dragonMesh = _resourceManager->getMesh(meshID);
	std::cout << "Models loaded.\n";

	size_t matID = _resourceManager->loadMaterial("redMat");
	std::shared_ptr<Material> redMat = _resourceManager->getMaterial(matID);
	redMat->addShader(simpleShader);
	redMat->ambient = glm::vec3(0.2f, 0.0f, 0.0f);
	redMat->diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
	redMat->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	redMat->shininess = 32.0f;

	matID = _resourceManager->loadMaterial("greenCelMat");
	std::shared_ptr<Material> greenMat = _resourceManager->getMaterial(matID);
	greenMat->addShader(_resourceManager->getShader("cel"));
	greenMat->addShader(_resourceManager->getShader("outline"));
	greenMat->ambient = glm::vec3(0.0f, 0.2f, 0.0f);
	greenMat->diffuse = glm::vec3(0.0f, 0.8f, 0.0f);
	greenMat->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	greenMat->shininess = 32.0f;

	matID = _resourceManager->loadMaterial("blueNormalMat");
	std::shared_ptr<Material> blueMat = _resourceManager->getMaterial(matID);
	blueMat->addShader(simpleShader);
	blueMat->addShader(_resourceManager->getShader("normal"));
	blueMat->ambient = glm::vec3(0.0f, 0.0f, 0.2f);
	blueMat->diffuse = glm::vec3(0.0f, 0.0f, 0.8f);
	blueMat->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	blueMat->shininess = 32.0f;

	matID = _resourceManager->loadMaterial("texture");
	std::shared_ptr<Material> texMaterial = _resourceManager->getMaterial(matID);
	texMaterial->addShader(_resourceManager->getShader("texture"));
	texMaterial->texture = _resourceManager->getTexture("texture");

	// Initialize the scene
	float aspect = static_cast<float>(_fbWidth) / static_cast<float>(_fbHeight);
	_camera = std::make_shared<Camera>(glm::vec3(-5.0f, 0.0f, 0.0f), aspect);
	_scene.setCamera(_camera);

	auto dirLight = std::make_shared<DirectionalLight>(glm::vec3(0.8f, -1.0f, 0.6f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	_scene.addLight(dirLight);

	auto plane = std::make_shared<Object>(planeMesh, texMaterial);
	plane->transform.translation = glm::vec3(0.0f, -1.0f, 0.0f);
	plane->transform.scale = glm::vec3(10.0f, 1.0f, 10.0f);
	_scene.addObject(plane);
	
	auto armadillo = std::make_shared<Object>(armadilloMesh, greenMat);
	armadillo->transform.scale = glm::vec3(0.020f);
	_scene.addObject(armadillo);

	auto dragon = std::make_shared<Object>(dragonMesh, blueMat);
	dragon->transform.translation = glm::vec3(0.0f, 0.0f, -5.0f);
	dragon->transform.scale = glm::vec3(20.0f);
	_scene.addObject(dragon);

	auto cube2 = std::make_shared<Object>(cubeMesh, redMat);
	cube2->transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	_scene.addObject(cube2);

	// Initialize render pipeline
	_renderPipeline.init(_scene, _fbWidth, _fbHeight);

	auto forwardPass = std::make_unique<ForwardRenderPass>();
	forwardPass->outlineShader = _resourceManager->getShader("outline");
	_renderPipeline.addRenderPass(std::move(forwardPass));

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Application::run()
{
	// Update camera
	_camera->updatePosition(_translateInput, Time::deltaTime());
	_camera->updateRotation(_rotateInput);
	_rotateInput = glm::vec3(0.0f);

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