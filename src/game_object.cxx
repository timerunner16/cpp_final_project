extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "game_object.hpp"

GameObject::GameObject(std::string script_path) {
	m_script_path = script_path;
}

GameObject::~GameObject() {}

void GameObject::Process() {
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, m_script_path.c_str());
	lua_pcall(L,0,0,0);
	lua_getglobal(L, "process");
	if (lua_isfunction(L, -1)) {
		lua_pcall(L,0,0,0);
	}
	lua_close(L);
}
