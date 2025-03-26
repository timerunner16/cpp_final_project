#include "game_object.hpp"

GameObject::GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform) {
	m_script_path = script_path;

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;
	
	m_lua_state = sol::state();
	m_lua_state.open_libraries(sol::lib::base);

	m_lua_state.script_file(script_path, sol::load_mode::text);

	m_lua_state["init"]();
}

GameObject::~GameObject() {}

void GameObject::Process() {
	m_lua_state["process"]();
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
