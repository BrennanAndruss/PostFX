#include "UniformBuffer.h"

void UniformBuffer::init(size_t size, GLuint bindingPoint)
{
	// Initialize UBO and allocate memory
	glGenBuffers(1, &_uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::update(const void* data, size_t size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, _uboID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}