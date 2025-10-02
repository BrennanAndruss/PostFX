#pragma once

#include <vector>
#include <memory>
#include "Shader.h"
#include "Texture.h"

struct Material
{
	Material() = default;
	~Material() = default;

	void addShader(std::shared_ptr<Shader> shader) { shaders.push_back(shader); }

	void setUniforms(std::shared_ptr<Shader> shader) const;
	void bindTexture(std::shared_ptr<Shader> shader) const;

	std::vector<std::shared_ptr<Shader>> shaders;
	std::shared_ptr<Texture> texture;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};