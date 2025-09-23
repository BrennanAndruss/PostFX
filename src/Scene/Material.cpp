#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader) : shader(shader), ambient(),
	diffuse(), specular(), shininess(1.0f) {}

void Material::setUniforms(std::shared_ptr<Shader> shader) const
{
	shader->setVec3("matAmb", ambient);
	shader->setVec3("matDiff", diffuse);
	shader->setVec3("matSpec", specular);
	shader->setFloat("matShine", shininess);
}