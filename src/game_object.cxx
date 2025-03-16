extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "game_object.hpp"

GameObject::GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform) {
	m_script_path = script_path;

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;

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

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
