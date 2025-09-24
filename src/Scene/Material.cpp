#include "Material.h"
#include <iostream>

Material::Material(std::shared_ptr<Shader> shader) 
	: shader(shader), texture(nullptr), ambient(), diffuse(), specular(), shininess(32.0f) {}

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
	std::cout << "handle: " << handle << std::endl;
	texture->bind(handle);
}