#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::getCompositeTransform() const
{
	glm::mat4 model(1.0f);

	model = glm::translate(model, translation);
	
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	return model;
}

Object::Object(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	: transform({ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f) }), material(material), mesh(mesh) {}

void Object::draw()
{
	mesh->draw();
}