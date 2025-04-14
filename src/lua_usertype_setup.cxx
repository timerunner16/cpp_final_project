#include "lua_usertype_setup.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "transform.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "input.hpp"

int engine_globals_deny(lua_State* L) {
	return luaL_error(L, "Can't modify Engine global table.");
}

int keys_deny(lua_State* L) {
	return luaL_error(L, "Can't modify Keys table.");
}

void lua_usertype_setup(Game *game, sol::state& lua_state, GameObject* game_object) {
	sol::usertype<vec2> vec2_data_type = lua_state.new_usertype<vec2>(
		"Vector2",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec2>();
			},
			[](const float& value) {
				return std::make_shared<vec2>(value);
			},
			[](const float& x, const float& y) {
				return std::make_shared<vec2>(x, y);
			},
			[](const ivec2& obj) {
				return std::make_shared<vec2>(obj);
			}
		),
		"x", sol::property(&vec2::get_x, &vec2::set_x),
		"y", sol::property(&vec2::get_y, &vec2::set_y),
		"plus", &vec2::add,
		"minus", &vec2::sub,
		"times", sol::overload(&vec2::mul, &vec2::mulf),
		"div", sol::overload(&vec2::div, &vec2::divf),
		"length", sol::readonly_property(&vec2::length),
		"unit", sol::readonly_property(&vec2::unit),
		"dot", &vec2::dot
	);

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
			},
			[](const ivec3& obj) {
				return std::make_shared<vec3>(obj);
			}
		),
		"x", sol::property(&vec3::get_x, &vec3::set_x),
		"y", sol::property(&vec3::get_y, &vec3::set_y),
		"z", sol::property(&vec3::get_z, &vec3::set_z),
		"plus", &vec3::add,
		"minus", &vec3::sub,
		"times", sol::overload(&vec3::mul, &vec3::mulf),
		"div", sol::overload(&vec3::div, &vec3::divf),
		"length", sol::readonly_property(&vec3::length),
		"unit", sol::readonly_property(&vec3::unit),
		"dot", &vec3::dot
	);

	sol::usertype<ivec2> ivec2_data_type = lua_state.new_usertype<ivec2>(
		"IVector2",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec2>();
			},
			[](const int& value) {
				return std::make_shared<vec2>(value);
			},
			[](const int& x, const int& y) {
				return std::make_shared<vec2>(x, y);
			},
			[](const vec2& obj) {
				return std::make_shared<ivec2>(obj);
			}
		),
		"x", sol::property(&ivec2::get_x, &ivec2::set_x),
		"y", sol::property(&ivec2::get_y, &ivec2::set_y),
		"plus", &ivec2::add,
		"minus", &ivec2::sub,
		"times", sol::overload(&ivec2::mul, &ivec2::mulf),
		"div", sol::overload(&ivec2::div, &ivec2::divf),
		"length", &ivec2::length
	);

	sol::usertype<ivec3> ivec3_data_type = lua_state.new_usertype<ivec3>(
		"IVector3",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec3>();
			},
			[](const int& value) {
				return std::make_shared<vec3>(value);
			},
			[](const int& x, const int& y, const int& z) {
				return std::make_shared<vec3>(x, y, z);
			},
			[](const vec3& obj) {
				return std::make_shared<ivec3>(obj);
			}
		),
		"x", sol::property(&ivec3::get_x, &ivec3::set_x),
		"y", sol::property(&ivec3::get_y, &ivec3::set_y),
		"z", sol::property(&ivec3::get_z, &ivec3::set_z),
		"plus", &ivec3::add,
		"minus", &ivec3::sub,
		"times", &ivec3::mul,
		"div", &ivec3::div,
		"length", &ivec3::length
	);


	sol::usertype<Transform> transform_data_type = lua_state.new_usertype<Transform>("Transform",
		sol::constructors<Transform(vec3,vec3,vec3)>());

	transform_data_type["position"] = sol::property(&Transform::get_position, &Transform::set_position);
	transform_data_type["rotation"] = sol::property(&Transform::get_rotation, &Transform::set_rotation);
	transform_data_type["scale"] = sol::property(&Transform::get_scale, &Transform::set_scale);

	transform_data_type["LookVector"] = sol::readonly_property(&Transform::GetLookVector);
	transform_data_type["RightVector"] = sol::readonly_property(&Transform::GetRightVector);
	transform_data_type["UpVector"] = sol::readonly_property(&Transform::GetUpVector);


	sol::usertype<GameObject> game_object_data_type = lua_state.new_usertype<GameObject>("GameObject",sol::no_constructor);

	game_object_data_type["transform"] = sol::property(&GameObject::GetTransform, &GameObject::SetTransform);
	game_object_data_type["global_transform"] = sol::readonly_property(&GameObject::GetGlobalTransform);
	game_object_data_type.set_function("GetChild", &GameObject::GetChild);
	game_object_data_type.set_function("GetChildren", &GameObject::GetChildren_Lua);
	game_object_data_type.set_function("GetParent", &GameObject::GetParent);
	game_object_data_type.set_function("GetName", &GameObject::GetName);


	sol::usertype<Camera> camera_data_type = lua_state.new_usertype<Camera>("Camera", sol::no_constructor);
	
	camera_data_type["transform"] = sol::property(&Camera::GetTransform, &Camera::SetTransform);


	sol::usertype<ButtonState> button_state_data_type = lua_state.new_usertype<ButtonState>(
		"ButtonState",
		sol::no_constructor,
		"Pressed", sol::readonly_property(&ButtonState::GetPressed),
		"OnEdge", sol::readonly_property(&ButtonState::GetOnEdge)
	);


	sol::usertype<InputManager> input_manager_data_type = lua_state.new_usertype<InputManager>("InputManager",sol::no_constructor);

	input_manager_data_type.set_function("QueryKey", &InputManager::QueryKey);
	input_manager_data_type.set_function("QueryMouseButton", &InputManager::QueryMouseButton);
	input_manager_data_type.set_function("GetMousePos", &InputManager::GetMousePos);
	input_manager_data_type.set_function("GetMouseDelta", &InputManager::GetMouseDelta);
	
	input_manager_data_type.set_function("SetMouseCaptured", &InputManager::SetMouseCaptured);


	sol::table keys_table = lua_state.create_named_table("Keys");

	sol::table keys_metatable = lua_state.create_table_with();
	keys_metatable["A"] = SDL_SCANCODE_A;
	keys_metatable["B"] = SDL_SCANCODE_B;
	keys_metatable["C"] = SDL_SCANCODE_C;
	keys_metatable["D"] = SDL_SCANCODE_D;
	keys_metatable["E"] = SDL_SCANCODE_E;
	keys_metatable["F"] = SDL_SCANCODE_F;
	keys_metatable["G"] = SDL_SCANCODE_G;
	keys_metatable["H"] = SDL_SCANCODE_H;
	keys_metatable["I"] = SDL_SCANCODE_I;
	keys_metatable["J"] = SDL_SCANCODE_J;
	keys_metatable["K"] = SDL_SCANCODE_K;
	keys_metatable["L"] = SDL_SCANCODE_L;
	keys_metatable["M"] = SDL_SCANCODE_M;
	keys_metatable["N"] = SDL_SCANCODE_N;
	keys_metatable["O"] = SDL_SCANCODE_O;
	keys_metatable["P"] = SDL_SCANCODE_P;
	keys_metatable["Q"] = SDL_SCANCODE_Q;
	keys_metatable["R"] = SDL_SCANCODE_R;
	keys_metatable["S"] = SDL_SCANCODE_S;
	keys_metatable["T"] = SDL_SCANCODE_T;
	keys_metatable["U"] = SDL_SCANCODE_U;
	keys_metatable["V"] = SDL_SCANCODE_V;
	keys_metatable["W"] = SDL_SCANCODE_W;
	keys_metatable["X"] = SDL_SCANCODE_X;
	keys_metatable["Y"] = SDL_SCANCODE_Y;
	keys_metatable["Z"] = SDL_SCANCODE_Z;

	keys_metatable["0"] = SDL_SCANCODE_0;
	keys_metatable["1"] = SDL_SCANCODE_1;
	keys_metatable["2"] = SDL_SCANCODE_2;
	keys_metatable["3"] = SDL_SCANCODE_3;
	keys_metatable["4"] = SDL_SCANCODE_4;
	keys_metatable["5"] = SDL_SCANCODE_5;
	keys_metatable["6"] = SDL_SCANCODE_6;
	keys_metatable["7"] = SDL_SCANCODE_7;
	keys_metatable["8"] = SDL_SCANCODE_8;
	keys_metatable["9"] = SDL_SCANCODE_9;

	keys_metatable["Return"] = SDL_SCANCODE_RETURN;
	keys_metatable["Escape"] = SDL_SCANCODE_ESCAPE;
	keys_metatable["Backspace"] = SDL_SCANCODE_BACKSPACE;
	keys_metatable["Tab"] = SDL_SCANCODE_TAB;
	keys_metatable["Space"] = SDL_SCANCODE_SPACE;
	
	keys_metatable["Minus"] = SDL_SCANCODE_MINUS;
	keys_metatable["Equals"] = SDL_SCANCODE_EQUALS;
	keys_metatable["LeftBracket"] = SDL_SCANCODE_LEFTBRACKET;
	keys_metatable["RightBracket"] = SDL_SCANCODE_RIGHTBRACKET;
	keys_metatable["Backslash"] = SDL_SCANCODE_BACKSLASH;

	keys_metatable["Semicolon"] = SDL_SCANCODE_SEMICOLON;
	keys_metatable["Apostrophe"] = SDL_SCANCODE_APOSTROPHE;
	keys_metatable["Grave"] = SDL_SCANCODE_GRAVE;
	keys_metatable["Comma"] = SDL_SCANCODE_COMMA;
	keys_metatable["Period"] = SDL_SCANCODE_PERIOD;
	keys_metatable["Slash"] = SDL_SCANCODE_SLASH;
	
	keys_metatable["CapsLock"] = SDL_SCANCODE_CAPSLOCK;

	keys_metatable["LShift"] = SDL_SCANCODE_LSHIFT;
	keys_metatable["RShift"] = SDL_SCANCODE_RSHIFT;
	keys_metatable["LControl"] = SDL_SCANCODE_LCTRL;
	keys_metatable["RControl"] = SDL_SCANCODE_RCTRL;
	keys_metatable["LAlt"] = SDL_SCANCODE_LALT;
	keys_metatable["RAlt"] = SDL_SCANCODE_RALT;

	keys_metatable["Left"] = SDL_SCANCODE_LEFT;
	keys_metatable["Right"] = SDL_SCANCODE_RIGHT;
	keys_metatable["Up"] = SDL_SCANCODE_UP;
	keys_metatable["Down"] = SDL_SCANCODE_DOWN;

	keys_metatable[sol::meta_function::new_index] = keys_deny;
	keys_metatable[sol::meta_function::index] = keys_metatable;

	keys_table[sol::metatable_key] = keys_metatable;


	sol::usertype<Window> window_data_type = lua_state.new_usertype<Window>(
		"Window",
		sol::no_constructor,
		"Width", sol::readonly_property(&Window::GetWidth),
		"Height", sol::readonly_property(&Window::GetHeight)
	);
	

	sol::usertype<Workspace> workspace_data_type = lua_state.new_usertype<Workspace>("Workspace", sol::no_constructor);
	
	workspace_data_type.set_function("GetGameObject", &Workspace::GetGameObject);
	workspace_data_type.set_function("GetGameObjects", &Workspace::GetGameObjects_Lua);
	workspace_data_type.set_function("GetCamera", &Workspace::GetCamera);


	sol::table engine_globals_table = lua_state.create_named_table("Engine");

	sol::table engine_globals_metatable = lua_state.create_table_with();
	engine_globals_metatable["Window"] = game->GetWindow();
	engine_globals_metatable["Workspace"] = game->GetWorkspace();
	engine_globals_metatable["InputManager"] = game->GetInputManager();
	engine_globals_metatable["CurrentGameObject"] = game_object;

	engine_globals_metatable[sol::meta_function::new_index] = engine_globals_deny;
	engine_globals_metatable[sol::meta_function::index] = engine_globals_metatable;

	engine_globals_table[sol::metatable_key] = engine_globals_metatable;
}
