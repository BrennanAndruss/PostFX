#include "Material.h"
#include <iostream>

void Material::setUniforms(std::shared_ptr<Shader> shader) const
{
	shader->setVec3("matAmb", ambient);
	shader->setVec3("matDif", diffuse);
	shader->setVec3("matSpec", specular);
	shader->setFloat("matShine", shininess);
}

void Material::bindTexture(std::shared_ptr<Shader> shader) const
{
	if (!texture)
		return;
		
	// Get the handle for the texture uniform
	GLint handle = glGetUniformLocation(shader->getPid(), "Texture0");
	texture->bind(handle);
}