#include "ForwardRenderPass.h"
#include <iostream>

void ForwardRenderPass::init()
{
	_resourceManager = ResourceManager::getInstance();
}

void ForwardRenderPass::execute(const Scene& scene)
{
	auto objects = scene.getObjects();
	int numLights = scene.getLights().size();

	for (auto object : objects)
	{
		for (auto& shader : object->material->shaders)
		{
			if (shader == outlineShader)
			{
				glCullFace(GL_FRONT);
			}

			shader->bind();

			object->material->setUniforms(shader);
			shader->setMat4("model", object->transform.getCompositeTransform());
			shader->setInt("numLights", numLights);
			object->material->bindTexture(shader);

			object->draw();

			shader->unbind();

			if (shader == outlineShader)
			{
				glCullFace(GL_BACK);
			}
		}
	}
}
