#include "pdmgr_usertype.hpp"
#include "pdata_manager.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "particle_system.hpp"
#include "transform.hpp"
#include "physics.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "material.hpp"

void pdmgr_usertype_setup(std::shared_ptr<sol::state> lua_state) {
	sol::usertype<PDataManager> pdata_manager_data_type = lua_state->new_usertype<PDataManager>(
		"PDataManager", sol::no_constructor,
		"GetValue", &PDataManager::GetPData,
		"SetValue", &PDataManager::SetPData
	);
}
