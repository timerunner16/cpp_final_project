#include "engine_globals.hpp"
#include "game.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "input.hpp"
#include "resource_manager.hpp"
#include "game_object.hpp"

int engine_globals_deny(lua_State* L) {
	return luaL_error(L, "Can't modify Engine global table.");
}

void engine_globals(Game* game, std::shared_ptr<sol::state> lua_state, GameObject* game_object) {
	sol::table engine_globals_table = lua_state->create_named_table("Engine");

	sol::table engine_globals_metatable = lua_state->create_table_with();
	engine_globals_metatable["Window"] = game->GetWindow();
	engine_globals_metatable["Workspace"] = game->GetWorkspace();
	engine_globals_metatable["InputManager"] = game->GetInputManager();
	engine_globals_metatable["ResourceManager"] = game->GetResourceManager();
	engine_globals_metatable["CurrentGameObject"] = game_object;

	engine_globals_metatable[sol::meta_function::new_index] = engine_globals_deny;
	engine_globals_metatable[sol::meta_function::index] = engine_globals_metatable;

	engine_globals_table[sol::metatable_key] = engine_globals_metatable;
}
