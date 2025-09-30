#pragma once

#include <memory>

#include "RenderPass.h"
#include "../Application/ResourceManager.h"
#include "../Scene/Shader.h"

class ForwardRenderPass : public RenderPass
{
public:
	void init() override;
	void execute(const Scene& scene) override;

private:
	ResourceManager* _resourceManager = nullptr;

	std::shared_ptr<Shader> _shader;
	Mesh _mesh;

	std::vector<float> _vertices;
	GLuint _VAO, _VBO;
};