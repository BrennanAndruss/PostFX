#pragma once

#include <vector>
#include <memory>

#include "RenderPass.h"

class RenderPipeline
{
public:
	RenderPipeline() = default;
	~RenderPipeline() = default;

	void resize(int width, int height);
	void addRenderPass(std::unique_ptr<RenderPass> renderPass);
	void render();

private:
	std::vector<std::unique_ptr<RenderPass>> _renderPasses;

	int _fbWidth;
	int _fbHeight;
};