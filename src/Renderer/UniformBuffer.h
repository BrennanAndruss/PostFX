#pragma once

#include <glad/glad.h>

class UniformBuffer
{
public:
	UniformBuffer() = default;
	~UniformBuffer() = default;
	
	void init(size_t size, GLuint bindingPoint);
	void update(const void* data, size_t size);

private:
	GLuint _uboID;
};