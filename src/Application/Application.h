#pragma once

#include <glad/glad.h>

#include "WindowManager.h"
#include "ResourceManager.h"
#include "../Renderer/RenderPipeline.h"
#include "../Scene/Scene.h"

class Application : public EventCallbacks
{
public:
	Application();
	~Application();

	void init(int screenWidth, int screenHeight);
	void run();
	bool shouldClose();

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void resizeCallback(GLFWwindow* window, int width, int height);
	void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
	WindowManager* _windowManager = nullptr;
	ResourceManager* _resourceManager = nullptr;

	RenderPipeline _renderPipeline;
	Scene _scene;
	std::shared_ptr<Camera> _camera;

	glm::vec3 _translateInput;
	glm::vec2 _rotateInput;
	bool _mouseTrapped;
	bool _firstMouse;

	int _fbWidth;
	int _fbHeight;
};