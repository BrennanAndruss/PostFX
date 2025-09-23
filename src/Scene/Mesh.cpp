#include "Mesh.h"

void Mesh::init(const tinyobj::shape_t& shape)
{
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	// Allocate space for the data in a single buffer
	size_t numVertices = shape.mesh.positions.size() / 3;
	_vertBuf.resize(numVertices * VERTEX_SIZE);

	// Fill the buffer with the vertex data
	for (size_t i = 0; i < numVertices; i++)
	{
		size_t index = VERTEX_SIZE * i;
		_vertBuf[index] = shape.mesh.positions[3 * i];
		_vertBuf[index + 1] = shape.mesh.positions[3 * i + 1];
		_vertBuf[index + 2] = shape.mesh.positions[3 * i + 2];
		_vertBuf[index + 3] = shape.mesh.normals[3 * i];
		_vertBuf[index + 4] = shape.mesh.normals[3 * i + 1];
		_vertBuf[index + 5] = shape.mesh.normals[3 * i + 2];
		if (!shape.mesh.texcoords.empty())
		{
			_vertBuf[index + 6] = shape.mesh.texcoords[2 * i];
			_vertBuf[index + 7] = shape.mesh.texcoords[2 * i + 1];
		}
	}

	// Bind the vertex data to the buffer
	glBufferData(GL_ARRAY_BUFFER, _vertBuf.size() * sizeof(float), _vertBuf.data(), GL_STATIC_DRAW);

	// Configure the vertex attributes
	size_t stride = VERTEX_SIZE * sizeof(float);
	size_t offset = 0;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += 3 * sizeof(float);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += 3 * sizeof(float);

	if (!shape.mesh.texcoords.empty())
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	}

	// Initialize the element buffer object
	_numIndices = shape.mesh.indices.size();
	glGenBuffers(1, &_eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(size_t), shape.mesh.indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Initialize the bounding box
	std::vector<float> positions = shape.mesh.positions;
	_bbox.min = glm::vec3(FLT_MAX);
	_bbox.max = glm::vec3(FLT_MIN);

	for (size_t i = 0; i < positions.size(); i += 3)
	{
		_bbox.min.x = std::min(_bbox.min.x, positions[i]);
		_bbox.min.y = std::min(_bbox.min.y, positions[i + 1]);
		_bbox.min.z = std::min(_bbox.min.z, positions[i + 2]);
		_bbox.max.x = std::max(_bbox.max.x, positions[i]);
		_bbox.max.y = std::max(_bbox.max.y, positions[i + 1]);
		_bbox.max.z = std::max(_bbox.max.z, positions[i + 2]);
	}
}

BBox Mesh::getBBox() const
{
	return _bbox;
}

void Mesh::draw() const
{
	glBindVertexArray(_vaoID);
	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
