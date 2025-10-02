#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader() = default;
	~Shader();

	void init(const std::string& name, const char* vertSource, const char* fragSource, const char* geomSource);
	GLuint getPid() const;

	void bind() const;
	void unbind() const;

	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, const glm::vec2& value) const;
	void setVec3(const char* name, const glm::vec3& value) const;
	void setVec4(const char* name, const glm::vec4& value) const;
	void setMat2(const char* name, const glm::mat2& value) const;
	void setMat3(const char* name, const glm::mat3& value) const;
	void setMat4(const char* name, const glm::mat4& value) const;

private:
	GLuint _pid;
};