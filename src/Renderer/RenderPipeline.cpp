#include "RenderPipeline.h"

#include <glfw/glfw3.h>
#include "../Scene/Camera.h"

void RenderPipeline::init(const Scene& scene, int width, int height)
{
	_fbWidth = width;
	_fbHeight = height;

	// Initialize uniform buffer objects with scene data
	std::shared_ptr<Camera> camera = scene.getCamera();
	_cameraUBO.init(sizeof(CameraUBO), 0);
	updateCameraUBO(*camera);
	_lightsUBO.init(sizeof(glm::vec3), 1);
	updateLightsUBO(glm::vec3(0.8f, -1.0f, 0.6f));
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
	std::vector<std::shared_ptr<Object>> objects = scene.getObjects();
	for (const auto& pass : _renderPasses)
	{
		pass->execute(objects);
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

void RenderPipeline::updateLightsUBO(const glm::vec3& lightDir)
{
	_lightsUBO.update(&lightDir, sizeof(glm::vec3));
}