#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../Scene/Material.h"
#include "../Scene/Mesh.h"

#include "../Scene/Shader.h"

class ResourceManager
{
public:
	static ResourceManager* getInstance();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	void setResourceDir(const std::string& resourceDir);

	size_t loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	size_t loadMesh(const std::string& name, const std::string& meshPath);
	size_t loadMaterial(const std::string& name, const std::string& shaderName);

	std::shared_ptr<Shader> getShader(size_t id) const;
	std::shared_ptr<Shader> getShader(const std::string& name) const;

	std::shared_ptr<Mesh> getMesh(size_t id) const;
	std::shared_ptr<Mesh> getMesh(const std::string& name) const;

	std::shared_ptr<Material> getMaterial(size_t id) const;
	std::shared_ptr<Material> getMaterial(const std::string& name) const;

private:
	ResourceManager();
	~ResourceManager();

	std::string _resourceDir;

	std::vector<std::shared_ptr<Shader>> _shaders;
	std::vector<std::shared_ptr<Mesh>> _meshes;
	std::vector<std::shared_ptr<Material>> _materials;

	std::map<std::string, size_t> _shaderIDs;
	std::map<std::string, size_t> _meshIDs;
	std::map<std::string, size_t> _materialIDs;
};