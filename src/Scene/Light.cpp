#include "Light.h"

void DirectionalLight::toUBO(LightUBO& ubo) const
{
	ubo.color_intensity = glm::vec4(color, intensity);
	ubo.direction_type = glm::vec4(direction, static_cast<float>(LightType::Directional));
}

void PointLight::toUBO(LightUBO& ubo) const
{
	ubo.color_intensity = glm::vec4(color, intensity);
	ubo.position_range = glm::vec4(position, range);
	ubo.direction_type = glm::vec4(0.0f, 0.0f, 0.0f, static_cast<float>(LightType::Point));
}