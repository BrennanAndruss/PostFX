#pragma once

#include "../Scene/Scene.h"

class RenderPass
{
public:
	virtual void init() = 0;
	virtual void execute(const Scene& scene) = 0;
};
