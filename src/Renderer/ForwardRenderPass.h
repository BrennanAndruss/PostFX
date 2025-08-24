#pragma once

#include <memory>

#include "RenderPass.h"
#include "../Application/ResourceManager.h"
#include "../Scene/Shader.h"

class ForwardRenderPass : public RenderPass
{
public:
	void init() override;
	void execute() override;

private:
	ResourceManager* _resourceManager = nullptr;

	Shader _shader;

	std::vector<float> _vertices;
	GLuint _VAO, _VBO;
};