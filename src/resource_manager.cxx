#include <memory>
#include <utility>
#include <SDL2/SDL_image.h>
#include "resource_manager.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"

ResourceManager::ResourceManager() {
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_resource_map = std::map<std::string, std::shared_ptr<GenericResource>>();
}

ResourceManager::~ResourceManager() {
	m_resource_map.clear();
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
template std::shared_ptr<Shader> ResourceManager::GetResource<Shader>(std::string);

void ResourceManager::ClearUnusedResources() {
	std::erase_if(m_resource_map, [](const auto& item){
		auto const& [key, val] = item;
		return (val.unique() && !val->GetPersistent());
	});
}
