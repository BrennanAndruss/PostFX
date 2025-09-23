#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

struct BBox
{
	glm::vec3 min;
	glm::vec3 max;
};

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;

	void init(const tinyobj::shape_t& shape);

	BBox getBBox() const;

	void draw() const;

private:
	std::vector<float> _vertBuf;
	size_t _numIndices;
	constexpr static size_t VERTEX_SIZE = 8;

	GLuint _vaoID, _vboID, _eboID;

	BBox _bbox;
};
