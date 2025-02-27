#include "workspace.hpp"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

Workspace::Workspace() {}
Workspace::~Workspace() {}

void Workspace::Test() {
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, "test.lua");
	lua_pcall(L,0,0,0);
	lua_getglobal(L, "process");
	if (lua_isfunction(L, -1)) {
		lua_pcall(L,0,0,0);
	}
	lua_close(L);
}
