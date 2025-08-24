#include "ForwardRenderPass.h"

void ForwardRenderPass::init()
{
	_resourceManager = ResourceManager::getInstance();
	_shader = _resourceManager->getShader("simple");

	// Test triangle data
	_vertices = {
		// positions		// colors
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// bottom left
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// bottom right
		 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// top
	};
	
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void ForwardRenderPass::execute()
{
	_shader.bind();

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	_shader.unbind();
}
