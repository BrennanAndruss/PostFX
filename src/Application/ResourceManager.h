#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../Scene/Shader.h"

class ResourceManager
{
public:
	static ResourceManager* getInstance();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	void setResourceDir(const std::string& resourceDir);

	size_t loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	Shader& getShader(size_t id);
	Shader& getShader(const std::string& name);

private:
	ResourceManager();
	~ResourceManager();

	std::string _resourceDir;

	std::vector<std::unique_ptr<Shader>> _shaders;

	std::map<std::string, size_t> _shaderIDs;
};