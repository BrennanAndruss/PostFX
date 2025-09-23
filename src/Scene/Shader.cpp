#include "Shader.h"

#include <iostream>

void checkCompileError(GLuint shader)
{
	GLint infoLogSize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
	
	if (infoLogSize > 0)
	{
		GLchar* infoLog = new GLchar[infoLogSize];
		if (infoLog == NULL)
		{
			std::cerr << "Could not allocate InfoLog buffer" << std::endl;
			exit(1);
		}

		glGetShaderInfoLog(shader, infoLogSize, NULL, infoLog);
		std::cerr << "ERROR::SHADER_COMPILATION_ERROR\n" << infoLog << std::endl;
		delete[] infoLog;
	}
}

void checkLinkError(GLuint program)
{
	GLint infoLogSize = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);

	if (infoLogSize > 0)
	{
		GLchar* infoLog = new GLchar[infoLogSize];
		if (infoLog == NULL)
		{
			std::cerr << "Could not allocate InfoLog buffer" << std::endl;
			exit(1);
		}

		glGetProgramInfoLog(program, infoLogSize, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
		delete[] infoLog;
	}
}

Shader::~Shader()
{
	glDeleteProgram(_pid);
}

void Shader::init(const char* vertSource, const char* fragSource)
{
	// Compile vertex shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);
	checkCompileError(vertShader);

	// Compile fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	checkCompileError(fragShader);

	// Create the program and link
	_pid = glCreateProgram();
	glAttachShader(_pid, vertShader);
	glAttachShader(_pid, fragShader);
	glLinkProgram(_pid);
	checkLinkError(_pid);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

GLuint Shader::getPid() const
{
	return _pid;
}

void Shader::bind() const
{
	glUseProgram(_pid);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(_pid, name), value);
}

void Shader::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(_pid, name), value);
}

void Shader::setVec2(const char* name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(_pid, name), 1, &value[0]);
}

void Shader::setVec3(const char* name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(_pid, name), 1, &value[0]);
}

void Shader::setVec4(const char* name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(_pid, name), 1, &value[0]);
}

void Shader::setMat2(const char* name, const glm::mat2& value) const
{
	glUniformMatrix2fv(glGetUniformLocation(_pid, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const char* name, const glm::mat3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(_pid, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const char* name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(_pid, name), 1, GL_FALSE, &value[0][0]);
}