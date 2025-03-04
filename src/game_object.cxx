extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "game_object.hpp"
#include "mesh.hpp"
#include "shader.hpp"

GameObject::GameObject(std::string script_path) {
	m_script_path = script_path;

	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, m_script_path.c_str());
	lua_pcall(L,0,0,0);
	lua_getglobal(L, "init");
	if (lua_isfunction(L,-1)) {
		lua_pcall(L,0,0,0);
	}
	lua_close(L);

	m_mesh = new Mesh(std::string(""));
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

void GameObject::SetMesh(Mesh* mesh) {
	m_mesh = mesh;
}

void GameObject::SetShader(Shader* shader) {
	m_shader = shader;
}

void GameObject::SetTransform(const Transform& transform) {
	m_transform = transform;
}

Mesh* GameObject::GetMesh() {
	return m_mesh;
}

Shader* GameObject::GetShader() {
	return m_shader;
}

Transform& GameObject::GetTransform() {
	return m_transform;
}
