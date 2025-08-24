#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:
	Shader() = default;
	~Shader();

	void init(const char* vertSource, const char* fragSource);
	GLuint getPid() const;

	void bind();
	void unbind();

	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, glm::vec2& value) const;
	void setVec3(const char* name, glm::vec3& value) const;
	void setVec4(const char* name, glm::vec4& value) const;
	void setMat2(const char* name, glm::mat2& value) const;
	void setMat3(const char* name, glm::mat3& value) const;
	void setMat4(const char* name, glm::mat4& value) const;

private:
	GLuint _pid;
};