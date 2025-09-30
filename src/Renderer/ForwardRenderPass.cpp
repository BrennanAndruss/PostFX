#include "ForwardRenderPass.h"
#include <iostream>

void ForwardRenderPass::init()
{
	_resourceManager = ResourceManager::getInstance();
	_shader = _resourceManager->getShader("simple");
}

void ForwardRenderPass::execute(const Scene& scene)
{
	auto objects = scene.getObjects();
	int numLights = scene.getLights().size();

	for (auto object : objects)
	{
		std::shared_ptr<Shader> objShader = object->material->shader;
		objShader->bind();

		object->material->setUniforms(objShader);
		objShader->setMat4("model", object->transform.getCompositeTransform());
		objShader->setInt("numLights", numLights);
		object->material->bindTexture(objShader);

		object->draw();

		objShader->unbind();
	}
}
