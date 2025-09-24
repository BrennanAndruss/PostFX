#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "../../ext/stb_image/stb_image.h"

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
	
	auto shader = std::make_shared<Shader>();
	shader->init(vertSource.c_str(), fragSource.c_str());
	_shaders.push_back(shader);

	size_t id = _shaders.size() - 1;
	_shaderIDs[name] = id;
	return id;
}

size_t ResourceManager::loadTexture(const std::string& name, const std::string& texturePath, bool alpha)
{
	// Load image data
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load((_resourceDir + texturePath).c_str(), &width, &height, &nrChannels, 0);

	auto texture = std::make_shared<Texture>(width, height, alpha);
	bool success = texture->init(textureData);
	if (!success)
	{
		std::cerr << "Failed to initialize texture: " << texturePath << std::endl;
		return -1;
	}
	stbi_image_free(textureData);
	_textures.push_back(texture);

	size_t id = _textures.size() - 1;
	_textureIDs[name] = id;
	return id;
}

size_t ResourceManager::loadMesh(const std::string& name, const std::string& meshPath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> objMaterials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, (_resourceDir + meshPath).c_str());

	if (!rc)
	{
		std::cerr << errStr << std::endl;
		return -1;
	}
	else
	{
		auto mesh = std::make_shared<Mesh>();
		mesh->init(shapes[0]);
		_meshes.push_back(mesh);

		size_t id = _meshes.size() - 1;
		_meshIDs[name] = id;
		return id;
	}
}

size_t ResourceManager::loadMaterial(const std::string& name, const std::string& shaderName)
{
	auto material = std::make_shared<Material>(getShader(shaderName));
	_materials.push_back(std::move(material));

	size_t id = _materials.size() - 1;
	_materialIDs[name] = id;
	return id;
}

std::shared_ptr<Shader> ResourceManager::getShader(size_t id) const
{
	if (id >= _shaders.size())
	{
		throw std::out_of_range("Shader ID out of range");
	}
	return _shaders[id];
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name) const
{
	return getShader(_shaderIDs.at(name));
}

std::shared_ptr<Texture> ResourceManager::getTexture(size_t id) const
{
	if (id >= _textures.size())
	{
		throw std::out_of_range("Texture ID out of range");
	}
	return _textures[id];
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name) const
{
	return getTexture(_textureIDs.at(name));
}

std::shared_ptr<Mesh> ResourceManager::getMesh(size_t id) const
{
	if (id >= _meshes.size())
	{
		throw std::out_of_range("Mesh ID out of range");
	}
	return _meshes[id];
}

std::shared_ptr<Mesh> ResourceManager::getMesh(const std::string& name) const
{
	return getMesh(_meshIDs.at(name));
}

std::shared_ptr<Material> ResourceManager::getMaterial(size_t id) const
{
	if (id >= _materials.size())
	{
		throw std::out_of_range("Material ID out of range");
	}
	return _materials[id];
}

std::shared_ptr<Material> ResourceManager::getMaterial(const std::string& name) const
{
	return getMaterial(_materialIDs.at(name));
}