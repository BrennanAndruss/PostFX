#include "Texture.h"
#include <iostream>

Texture::Texture(int width, int height, bool alpha) : _width(width), _height(height), 
	_internalFormat(alpha ? GL_RGBA : GL_RGB), _imageFormat(alpha ? GL_RGBA : GL_RGB), _tid(0), _unit(0) {}

bool Texture::init(const unsigned char* textureData)
{
	// Generate a texture buffer object
	glGenTextures(1, &_tid);
	glBindTexture(GL_TEXTURE_2D, _tid);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load texture data
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _imageFormat, GL_UNSIGNED_BYTE, textureData);
	GLint w, h = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	if (w == 0 || h == 0)
	{
		std::cerr << "Failed to load texture" << std::endl;
		return false;
	}
	
	// Generate mipmaps and set texture parameters
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::bind(GLint handle) const
{
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(GL_TEXTURE_2D, _tid);
	glUniform1i(handle, _unit);
}

void Texture::unbind() const
{
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}