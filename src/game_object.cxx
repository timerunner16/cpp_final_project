extern "C" {
	#include "lualib.h"
	#include "lauxlib.h"
}
#include "game_object.hpp"

GameObject::GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform) {
	m_script_path = script_path;

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;

	m_lua_state = luaL_newstate();
	luaL_openlibs(m_lua_state);
	luaL_dofile(m_lua_state, m_script_path.c_str());
	lua_settop(m_lua_state, 0);
	lua_getglobal(m_lua_state, "init");
	if (lua_isfunction(m_lua_state,-1)) {
		lua_call(m_lua_state,0,0);
	}
}

GameObject::~GameObject() {
	lua_close(m_lua_state);
	m_lua_state = nullptr;
}

void GameObject::Process() {
	lua_getglobal(m_lua_state, "process");
	if (lua_isfunction(m_lua_state,-1)) {
		lua_call(m_lua_state,0,0);
	}
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
