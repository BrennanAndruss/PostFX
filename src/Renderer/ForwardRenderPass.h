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

	std::shared_ptr<Shader> outlineShader;

private:
	ResourceManager* _resourceManager = nullptr;
};