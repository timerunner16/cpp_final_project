#include "game_object.hpp"
#include "vec3.hpp"
#include "game.hpp"
#include "workspace.hpp"

GameObject::GameObject(Game* game, std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform) {
	m_game = game;

	m_name = name;

	m_parent = nullptr;
	SetParent(parent);

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;

	m_lua_loaded = !script_path.empty();
	if (!m_lua_loaded) return;
	m_lua_state = sol::state();
	m_lua_state.open_libraries(sol::lib::base, sol::lib::math);

	m_lua_state.script_file(script_path, sol::load_mode::text);

	sol::usertype<vec3> vec3_data_type = m_lua_state.new_usertype<vec3>("Vector3",
		sol::constructors<vec3(), vec3(float), vec3(float,float,float)>());

	vec3_data_type["x"] = sol::property(&vec3::get_x, &vec3::set_x);
	vec3_data_type["y"] = sol::property(&vec3::get_y, &vec3::set_y);
	vec3_data_type["z"] = sol::property(&vec3::get_z, &vec3::set_z);

	sol::usertype<Transform> transform_data_type = m_lua_state.new_usertype<Transform>("Transform",
		sol::constructors<Transform(vec3,vec3,vec3)>());

	transform_data_type["position"] = sol::property(&Transform::get_position, &Transform::set_position);
	transform_data_type["rotation"] = sol::property(&Transform::get_rotation, &Transform::set_rotation);
	transform_data_type["scale"] = sol::property(&Transform::get_scale, &Transform::set_scale);

	sol::usertype<GameObject> game_object_data_type = m_lua_state.new_usertype<GameObject>("GameObject",
		sol::constructors<>());

	game_object_data_type["transform"] = sol::property(&GameObject::GetTransform, &GameObject::SetTransform);
	game_object_data_type.set_function("GetChild", &GameObject::GetChild);

	m_lua_state.set_function("GetGameObject", &Workspace::GetGameObject, m_game->GetWorkspace());

	m_lua_state["_GameObject"] = this;

	sol::safe_function init = m_lua_state["init"];
	sol::protected_function_result result = init();
	if (!result.valid()) {
		sol::error error = result;
		printf("[Lua Error]: %s\n", error.what());
	}

	m_lua_process = m_lua_state["process"];
}

GameObject::~GameObject() {
	if (m_parent != nullptr) m_parent->RemoveChild(m_name);
	for (auto& [key, val] : m_children) {delete val;}
}

void GameObject::Process(float delta) {
	if (!m_lua_loaded) return;
	
	sol::protected_function_result result = m_lua_process(delta);
	if (!result.valid()) {
		sol::error error = result;
		printf("[LUA_ERROR]: %s\n", error.what());
	}
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::string GameObject::GetName() {return m_name;}
std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}

GameObject* GameObject::GetParent() {return m_parent;}
GameObject* GameObject::GetChild(std::string name) {
	if (m_children.contains(name)) return m_children[name];
	return nullptr;
}

void GameObject::SetParent(GameObject* parent) {
	if (m_parent != nullptr) {
		m_parent->RemoveChild(m_name);
	}
	if (parent != nullptr) {
		parent->AddChild(this);
	}
	m_parent = parent;
}

void GameObject::RemoveChild(std::string name) {
	if (m_children.contains(name)) m_children.erase(name);
}

void GameObject::AddChild(GameObject* child) {
	if (child == nullptr) return;
	m_children[child->GetName()] = child;
}
