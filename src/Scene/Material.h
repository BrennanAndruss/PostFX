#pragma once

#include <memory>
#include "Shader.h"

struct Material
{
	Material() = default;
	Material(std::shared_ptr<Shader> shader);
	~Material() = default;

	void setUniforms(std::shared_ptr<Shader> shader) const;

	std::shared_ptr<Shader> shader;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};