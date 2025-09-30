#pragma once

#include <glm/glm.hpp>

enum class LightType
{
	Directional,
	Point,
};

struct LightUBO
{
	glm::vec4 color_intensity;
	glm::vec4 position_range;
	glm::vec4 direction_type;
};

struct Light
{
	virtual ~Light() = default;
	virtual void toUBO(LightUBO& ubo) const = 0;

	glm::vec3 color;
	float intensity;
};

struct DirectionalLight : Light
{
	DirectionalLight() = default;
	DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
	{
		this->direction = glm::normalize(direction);
		this->color = color;
		this->intensity = intensity;
	}

	void toUBO(LightUBO& ubo) const override;

	glm::vec3 direction;
};

struct PointLight : Light
{
	PointLight() = default;
	PointLight(glm::vec3 position, float range, glm::vec3 color, float intensity)
	{
		this->position = position;
		this->range = range;
		this->color = color;
		this->intensity = intensity;
	}

	void toUBO(LightUBO& ubo) const override;

	glm::vec3 position;
	float range;
};