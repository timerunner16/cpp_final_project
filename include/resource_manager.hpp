#pragma once
#include <string>
#include <map>
#include <memory>

class Game;
class Material;
class GLTexture;
class Shader;
class Mesh;

class GenericResource {
public:
	~GenericResource() {Cleanup();};

	virtual void Cleanup() {}

	void SetPersistent(bool persistent);

	bool GetPersistent() {return m_persistent;}
	float GetTimeUnused() {return m_time_unused;}
	void IterateTime(float delta) {m_time_unused += delta;}
	void ResetTimeUnused() {m_time_unused=0.0f;}
private:
	bool m_persistent;

	float m_time_unused;
};

class ResourceManager {
public:
	ResourceManager(Game* game);
	~ResourceManager();

	void ClearUnusedResources();

	std::shared_ptr<Material> GetMaterial(std::string lumpname);
	std::shared_ptr<GLTexture> GetGLTexture(std::string lumpname);
	std::shared_ptr<Shader> GetShader(std::string lumpname);
	std::shared_ptr<Mesh> GetMesh(std::string lumpname);
private:
	std::map<std::string, std::shared_ptr<Material>> m_material_map;
	std::map<std::string, std::shared_ptr<GLTexture>> m_texture_map;
	std::map<std::string, std::shared_ptr<Shader>> m_shader_map;
	std::map<std::string, std::shared_ptr<Mesh>> m_mesh_map;
	Game* m_game;
};
