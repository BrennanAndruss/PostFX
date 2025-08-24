#pragma once

class RenderPass
{
public:
	virtual void init() = 0;
	virtual void execute() = 0;
};
