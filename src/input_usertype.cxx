#include "input_usertype.hpp"
#include "SDL2/SDL_scancode.h"
#include "input.hpp"

int keys_deny(lua_State* L) {
	return luaL_error(L, "Can't modify Keys table.");
}

void input_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject *game_object) {	
	sol::usertype<ButtonState> button_state_data_type = lua_state->new_usertype<ButtonState>(
		"ButtonState",
		sol::no_constructor,
		"Pressed", sol::readonly_property(&ButtonState::GetPressed),
		"OnEdge", sol::readonly_property(&ButtonState::GetOnEdge)
	);


	sol::usertype<InputManager> input_manager_data_type = lua_state->new_usertype<InputManager>(
		"InputManager", sol::no_constructor,
		"QueryKey", &InputManager::QueryKey,
		"QueryMouseButton", &InputManager::QueryMouseButton,
		"SomethingPressed", &InputManager::SomethingPressed,
		"GetAxis", &InputManager::GetAxis,
		"GetVector", &InputManager::GetVector,
		"GetMousePos", &InputManager::GetMousePos,
		"GetMouseDelta", &InputManager::GetMouseDelta,
		"SetMouseCaptured", &InputManager::SetMouseCaptured
	);


	sol::table keys_table = lua_state->create_named_table("Keys");

	sol::table keys_metatable = lua_state->create_table_with();
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
}
