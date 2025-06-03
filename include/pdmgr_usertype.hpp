#pragma once
#include <sol/sol.hpp>

class GameObject;

void pdmgr_usertype_setup(std::shared_ptr<sol::state> lua_state);
