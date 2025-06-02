#include "rsmgr_usertype.hpp"
#include "resource_manager.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "material.hpp"

void rsmgr_usertype_setup(std::shared_ptr<sol::state> lua_state) {
	sol::usertype<ResourceManager> resource_manager_data_type = lua_state->new_usertype<ResourceManager>(
		"ResourceManager", sol::no_constructor,
		"GetMesh", &ResourceManager::GetMesh,
		"GetTexture", &ResourceManager::GetGLTexture,
		"GetShader", &ResourceManager::GetShader,
		"GetMaterial", &ResourceManager::GetMaterial
	);
}
