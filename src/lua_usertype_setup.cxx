#include "lua_usertype_setup.hpp"
#include "game.hpp"
#include "vec3.hpp"
#include "transform.hpp"
#include "game_object.hpp"
#include "workspace.hpp"

int engine_globals_deny(lua_State* L) {
	return luaL_error(L, "Can't modify Engine global table.");
}

void lua_usertype_setup(Game *game, sol::state& lua_state, GameObject* game_object) {
	sol::usertype<vec3> vec3_data_type = lua_state.new_usertype<vec3>(
		"Vector3",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec3>();
			},
			[](const float& value) {
				return std::make_shared<vec3>(value);
			},
			[](const float& x, const float& y, const float& z) {
				return std::make_shared<vec3>(x, y, z);
			}
		),
		"x", sol::property(&vec3::get_x, &vec3::set_x),
		"y", sol::property(&vec3::get_y, &vec3::set_y),
		"z", sol::property(&vec3::get_z, &vec3::set_z)
	);
	vec3_data_type.set_function("add", &vec3::add);
	vec3_data_type.set_function("sub", &vec3::sub);
	vec3_data_type.set_function("mul", &vec3::mul);
	vec3_data_type.set_function("div", &vec3::div);

	vec3_data_type.set_function("mulf", &vec3::mulf);
	vec3_data_type.set_function("divf", &vec3::divf);

	sol::usertype<Transform> transform_data_type = lua_state.new_usertype<Transform>("Transform",
		sol::constructors<Transform(vec3,vec3,vec3)>());

	transform_data_type["position"] = sol::property(&Transform::get_position, &Transform::set_position);
	transform_data_type["rotation"] = sol::property(&Transform::get_rotation, &Transform::set_rotation);
	transform_data_type["scale"] = sol::property(&Transform::get_scale, &Transform::set_scale);

	sol::usertype<GameObject> game_object_data_type = lua_state.new_usertype<GameObject>("GameObject",
		sol::constructors<>());

	game_object_data_type["transform"] = sol::property(&GameObject::GetTransform, &GameObject::SetTransform);
	game_object_data_type.set_function("GetChild", &GameObject::GetChild);
	game_object_data_type.set_function("GetChildren", &GameObject::GetChildren_Lua);
	game_object_data_type.set_function("GetParent", &GameObject::GetParent);
	game_object_data_type.set_function("GetName", &GameObject::GetName);

	sol::usertype<Workspace> workspace_data_type = lua_state.new_usertype<Workspace>("Workspace", sol::constructors<>());
	
	workspace_data_type.set_function("GetGameObject", &Workspace::GetGameObject);
	workspace_data_type.set_function("GetGameObjects", &Workspace::GetGameObjects_Lua);

	sol::table engine_globals_table = lua_state.create_named_table("Engine");

	sol::table engine_globals_metatable = lua_state.create_table_with();
	engine_globals_metatable["Workspace"] = game->GetWorkspace();
	engine_globals_metatable["CurrentGameObject"] = game_object;

	engine_globals_metatable[sol::meta_function::new_index] = engine_globals_deny;
	engine_globals_metatable[sol::meta_function::index] = engine_globals_metatable;

	engine_globals_table[sol::metatable_key] = engine_globals_metatable;
}
