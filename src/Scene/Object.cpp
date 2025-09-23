#include "Object.h"

Object::Object(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	: transform({ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f) }), material(material), mesh(mesh) {}

void Object::draw()
{
	mesh->draw();
}