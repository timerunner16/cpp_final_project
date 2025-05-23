#pragma once
#include <sol/sol.hpp>

class GameObject;

void vdata_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject* game_object);
