#pragma once

#include <vector>
#include <memory>

#include "RenderPass.h"
#include "UniformBuffer.h"
#include "../Scene/Scene.h"

class RenderPipeline
{
public:
	RenderPipeline() = default;
	~RenderPipeline() = default;

	void init(const Scene& scene, int width, int height);
	void resize(int width, int height);
	void addRenderPass(std::unique_ptr<RenderPass> renderPass);
	void render(const Scene& scene);

private:
	std::vector<std::unique_ptr<RenderPass>> _renderPasses;
	
	int _fbWidth;
	int _fbHeight;

	UniformBuffer _cameraUBO;
	UniformBuffer _lightsUBO;

	void updateCameraUBO(const Camera& camera);
	void updateLightsUBO(const std::vector<std::shared_ptr<Light>>& lights);
};