#include "ForwardRenderPass.h"
#include <iostream>

void ForwardRenderPass::init()
{
	_resourceManager = ResourceManager::getInstance();
	_shader = _resourceManager->getShader("simple");

	//// Test triangle data
	//_vertices = {
	//-0.5f, -0.5f, 0.0f,
	// 0.5f, -0.5f, 0.0f,
	// 0.0f,  0.5f, 0.0f
	//};

	//glGenVertexArrays(1, &_VAO);
	//glGenBuffers(1, &_VBO);

	//glBindVertexArray(_VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void ForwardRenderPass::execute(const std::vector<std::shared_ptr<Object>>& objects)
{
	// _shader->bind();

	//GLint currentProgram = 0;
	//glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	//std::cout << "Program in use at draw: " << currentProgram << std::endl;

	//glBindVertexArray(_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	for (auto object : objects)
	{
		std::shared_ptr<Shader> objShader = object->material->shader;
		objShader->bind();

		object->material->setUniforms(objShader);
		objShader->setMat4("model", object->transform.getCompositeTransform());
		object->material->bindTexture(objShader);

		object->draw();

		objShader->unbind();
	}

	// _shader->unbind();
}
