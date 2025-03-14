extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "game_object.hpp"
#include "mesh.hpp"
#include "shader.hpp"


GameObject::GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, 
		std::shared_ptr<GLTexture> gl_texture, const Transform& transform) {
	m_script_path = script_path;

	m_mesh = mesh;
	m_shader = shader;
	m_transform = transform;
	m_gl_texture = gl_texture;

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
void GameObject::SetShader(std::shared_ptr<Shader> shader) {m_shader = shader;}
void GameObject::SetGLTexture(std::shared_ptr<GLTexture> gl_texture) {m_gl_texture = gl_texture;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Shader> GameObject::GetShader() {return m_shader;}
std::shared_ptr<GLTexture> GameObject::GetGLTexture() {return m_gl_texture;}
Transform& GameObject::GetTransform() {return m_transform;}
