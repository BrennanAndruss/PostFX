#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

struct Transform
{
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 getCompositeTransform() const;
};

struct Object
{
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	Object() = default;
	Object(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
	~Object() = default;

	void draw();
};