#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "Object.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	// Camera
	void setCamera(const std::shared_ptr<Camera>& camera) { _camera = camera; }
	std::shared_ptr<Camera> getCamera() const { return _camera; }

	// Objects
	void addObject(const std::shared_ptr<Object>& object) { _objects.push_back(object); }
	const std::vector<std::shared_ptr<Object>>& getObjects() const { return _objects; }

private:
	std::shared_ptr<Camera> _camera;
	std::vector<std::shared_ptr<Object>> _objects;
};