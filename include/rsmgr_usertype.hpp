#pragma once
#include <sol/sol.hpp>

class GameObject;

void rsmgr_usertype_setup(std::shared_ptr<sol::state> lua_state);
