#include "RenderPipeline.h"

#include <glfw/glfw3.h>

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

void RenderPipeline::render()
{
	glViewport(0, 0, _fbWidth, _fbHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& pass : _renderPasses)
	{
		pass->execute();
	}
}
