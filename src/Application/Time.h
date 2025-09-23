#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

class Time
{
public:
	static float time() { return _time; }
	static float deltaTime() { return _deltaTime; }

	static void update()
	{
		static double lastTime = 0.0;
		double currentTime = glfwGetTime();
		_time = static_cast<float>(currentTime);
		_deltaTime = _time - lastTime;
		lastTime = currentTime;
	}

private:
	static inline float _time = 0.0f;
	static inline float _deltaTime = 0.0f;
};