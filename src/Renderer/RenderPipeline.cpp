#include "RenderPipeline.h"

#include <glfw/glfw3.h>
#include "../Scene/Camera.h"
#include "../Scene/Light.h"

void RenderPipeline::init(const Scene& scene, int width, int height)
{
	_fbWidth = width;
	_fbHeight = height;

	// Initialize uniform buffer objects with scene data
	std::shared_ptr<Camera> camera = scene.getCamera();
	_cameraUBO.init(sizeof(CameraUBO), 0);
	updateCameraUBO(*camera);

	std::vector<std::shared_ptr<Light>> lights = scene.getLights();
	_lightsUBO.init(sizeof(LightUBO), lights.size());
	updateLightsUBO(lights);
}

void RenderPipeline::resize(int width, int height)
{
	_fbWidth = width;
	_fbHeight = height;
}

void RenderPipeline::addRenderPass(std::unique_ptr<RenderPass> renderPass)
{
	if (renderPass)
	{
		renderPass->init();
		_renderPasses.push_back(std::move(renderPass));
	}
}

void RenderPipeline::render(const Scene& scene)
{
	// Clear the previous frame
	glViewport(0, 0, _fbWidth, _fbHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update uniform buffer objects
	std::shared_ptr<Camera> camera = scene.getCamera();
	updateCameraUBO(*camera);

	// Execute render passes
	for (const auto& pass : _renderPasses)
	{
		pass->execute(scene);
	}
}

void RenderPipeline::updateCameraUBO(const Camera& camera)
{
	CameraUBO cameraData 
	{
		camera.getViewMatrix(),
		camera.getProjectionMatrix(),
		glm::vec4(camera.getPosition(), 0.0f)
	};
	_cameraUBO.update(&cameraData, sizeof(CameraUBO));
}

void RenderPipeline::updateLightsUBO(const std::vector<std::shared_ptr<Light>>& lights)
{
	std::vector<LightUBO> lightData;
	lightData.reserve(lights.size());
	for (const auto& light : lights)
	{
		LightUBO ubo;
		light->toUBO(ubo);
		lightData.push_back(ubo);
	}
	_lightsUBO.update(lightData.data(), lightData.size() * sizeof(LightUBO));
}