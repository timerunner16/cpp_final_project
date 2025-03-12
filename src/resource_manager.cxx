#include <memory>
#include <utility>
#include <SDL2/SDL_image.h>
#include "resource_manager.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"

ResourceManager::ResourceManager() {
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_resource_map = std::map<std::string, std::shared_ptr<GenericResource>>();
}

ResourceManager::~ResourceManager() {
	for (auto it = m_resource_map.begin(); it != m_resource_map.end(); it++) {
		DeleteResource(it->first);
	}
}

template <class T>
typename std::enable_if_t<std::constructible_from<T, std::string> && std::is_base_of_v<GenericResource, T>, std::shared_ptr<T>> ResourceManager::GetResource(std::string file_path) {
	if (m_resource_map.contains(file_path)) {
		std::shared_ptr<GenericResource> cached_resource = m_resource_map[file_path];
		if (cached_resource->GetType() == std::string(typeid(T).name())) return std::static_pointer_cast<T, GenericResource>(m_resource_map[file_path]);
		else {
			printf("Tried to interpret loaded asset %s with typeid %s as typeid %s.\n", file_path.c_str(), cached_resource->GetType().c_str(), typeid(T).name());
			return nullptr;
		}
	}
	
	T* resource = new T{file_path};
	m_resource_map.emplace(std::make_pair(file_path, resource));
	return std::static_pointer_cast<T, GenericResource>(m_resource_map[file_path]);
}

template std::shared_ptr<GLTexture> ResourceManager::GetResource<GLTexture>(std::string);
template std::shared_ptr<Mesh> ResourceManager::GetResource<Mesh>(std::string);

void ResourceManager::ClearUnusedResources() {
	for (auto it = m_resource_map.begin(); it != m_resource_map.end(); it++) {
		if (it->second.unique() && !it->second->GetPersistent()) {
			DeleteResource(it->first);
		}
	}
}

void ResourceManager::DeleteResource(std::string texture_path) {
	if (!m_resource_map.contains(texture_path)) return;

	m_resource_map.erase(texture_path);
}
