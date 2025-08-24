#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::string readFile(const std::string& filePath)
{
	std::string content;

	try
	{
		std::ifstream fileHandle(filePath);
		std::stringstream ss;
		ss << fileHandle.rdbuf();
		fileHandle.close();
		content = ss.str();
	}
	catch (std::exception& e)
	{
		std::cerr << "Could not open file: " << filePath << std::endl;
	}

	return content;
}

ResourceManager* ResourceManager::getInstance()
{
	static ResourceManager instance;
	return &instance;
}

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager()
{
	_shaders.clear();
	_shaderIDs.clear();
}

void ResourceManager::setResourceDir(const std::string& resourceDir)
{
	_resourceDir = resourceDir;
}

size_t ResourceManager::loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
{
	std::string vertSource = readFile(_resourceDir + vertPath);
	std::string fragSource = readFile(_resourceDir + fragPath);
	
	auto shader = std::make_unique<Shader>();
	shader->init(vertSource.c_str(), fragSource.c_str());
	_shaders.push_back(std::move(shader));

	size_t id = _shaders.size() - 1;
	_shaderIDs[name] = id;
	return id;
}

Shader& ResourceManager::getShader(size_t id)
{
	if (id >= _shaders.size())
	{
		throw std::out_of_range("Shader ID out of range");
	}
	return *(_shaders[id]);
}

Shader& ResourceManager::getShader(const std::string& name)
{
	return getShader(_shaderIDs[name]);
}
