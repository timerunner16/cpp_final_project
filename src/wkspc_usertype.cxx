#include "wkspc_usertype.hpp"
#include "workspace.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "event.hpp"
#include "particle_system.hpp"

void wkspc_usertype_setup(std::shared_ptr<sol::state> lua_state) {
	sol::usertype<Workspace> workspace_data_type = lua_state->new_usertype<Workspace>(
		"Workspace", sol::no_constructor,
		"GetGameObject", &Workspace::GetGameObject,
		"GetGameObjects", &Workspace::GetGameObjects_Lua,
		"GetCamera", &Workspace::GetCamera,
		"CreateGameObject", &Workspace::CreateGameObject,
		"CreateParticleSystem", &Workspace::CreateParticleSystem,
		"CreateEvent", &Workspace::CreateEvent
	);
}
