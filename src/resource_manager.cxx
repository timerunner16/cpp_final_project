#include <memory>
#include <utility>
#include <SDL2/SDL_image.h>
#include "resource_manager.hpp"
#include "material.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "game.hpp"
#include "parse_wad.hpp"
#include "util.hpp"

#define DECAY_TIME 10.0f

ResourceManager::ResourceManager(Game* game) {
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_game = game;
	m_material_map = std::map<std::string, std::shared_ptr<Material>>();
	m_texture_map = std::map<std::string, std::shared_ptr<GLTexture>>();
	m_shader_map = std::map<std::string, std::shared_ptr<Shader>>();
	m_mesh_map = std::map<std::string, std::shared_ptr<Mesh>>();
}

ResourceManager::~ResourceManager() {
	m_material_map.clear();
	m_texture_map.clear();
	m_shader_map.clear();
	m_mesh_map.clear();
}

std::shared_ptr<Material> ResourceManager::GetMaterial(std::string lumpname) {
	if (m_material_map.contains(lumpname)) {
		return m_material_map[lumpname];
	}
	
	lumpdata data = extract_lump_from_wad(m_game->GetWADPath(), lumpname, "M");
	if (!data.successful) return nullptr;
	std::string data_str((char*)data.data, data.size);
	std::vector<std::string> data_vec = split_string(data_str, "\n");
	Material* resource = new Material(m_game, m_game->GetWADPath(), data_vec);
	m_material_map.emplace(std::make_pair(lumpname, resource));
	delete data.data;
	return m_material_map[lumpname];
}

std::shared_ptr<GLTexture> ResourceManager::GetGLTexture(std::string lumpname) {
	if (m_texture_map.contains(lumpname)) {
		return m_texture_map[lumpname];
	}
	
	lumpdata data = extract_lump_from_wad(m_game->GetWADPath(), lumpname, "P", true);
	if (!data.successful) data = extract_lump_from_wad(m_game->GetWADPath(), lumpname, "F", false);
	if (!data.successful) return nullptr;
	GLTexture* resource = new GLTexture(data.data, data.size);
	m_texture_map.emplace(std::make_pair(lumpname, resource));
	delete data.data;
	return m_texture_map[lumpname];
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string lumpname) {
	if (m_shader_map.contains(lumpname)) {
		return m_shader_map[lumpname];
	}
	
	lumpdata data = extract_lump_from_wad(m_game->GetWADPath(), lumpname, "GL");
	if (!data.successful) return nullptr;
	Shader* resource = new Shader(data.data, data.size);
	m_shader_map.emplace(std::make_pair(lumpname, resource));
	delete data.data;
	return m_shader_map[lumpname];
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(std::string lumpname) {
	if (m_mesh_map.contains(lumpname)) {
		return m_mesh_map[lumpname];
	}
	
	lumpdata data = extract_lump_from_wad(m_game->GetWADPath(), lumpname, "MD");
	if (!data.successful) return nullptr;
	std::string data_str((char*)data.data, data.size);
	std::vector<std::string> data_vec = split_string(data_str, "\n");
	Mesh* resource = new Mesh(data_vec);
	m_mesh_map.emplace(std::make_pair(lumpname, resource));
	delete data.data;
	return m_mesh_map[lumpname];
}

void ResourceManager::ClearUnusedResources() {
	for (auto& [key, val] : m_material_map) {
		if (val.unique() && !val->GetPersistent()) val->IterateTime(m_game->GetDelta());
		else val->ResetTimeUnused();
	}
	for (auto& [key, val] : m_texture_map) {
		if (val.unique() && !val->GetPersistent()) val->IterateTime(m_game->GetDelta());
		else val->ResetTimeUnused();
	}
	for (auto& [key, val] : m_shader_map) {
		if (val.unique() && !val->GetPersistent()) val->IterateTime(m_game->GetDelta());
		else val->ResetTimeUnused();
	}
	for (auto& [key, val] : m_mesh_map) {
		if (val.unique() && !val->GetPersistent()) val->IterateTime(m_game->GetDelta());
		else val->ResetTimeUnused();
	}
	std::erase_if(m_material_map, [](const auto& item) -> bool {
		auto const& [key, val] = item;
		return (val->GetTimeUnused() >= DECAY_TIME);
	});
	std::erase_if(m_texture_map, [](const auto& item) -> bool {
		auto const& [key, val] = item;
		return (val->GetTimeUnused() >= DECAY_TIME);
	});
	std::erase_if(m_shader_map, [](const auto& item) -> bool {
		auto const& [key, val] = item;
		return (val->GetTimeUnused() >= DECAY_TIME);
	});
	std::erase_if(m_mesh_map, [](const auto& item) -> bool {
		auto const& [key, val] = item;
		return (val->GetTimeUnused() >= DECAY_TIME);
	});
}
