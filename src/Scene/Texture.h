#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture(int width, int height, bool alpha);
	~Texture() = default;

	bool init(const unsigned char* textureData);
	void setUnit(GLint u) { _unit = u; }
	GLint getUnit() const { return _unit; }

	void bind(GLint handle) const;
	void unbind() const;

private:
	int _width, _height;
	GLint _internalFormat;
	GLint _imageFormat;

	GLuint _tid;
	GLint _unit;
};