#include "game_object.hpp"
#include "game.hpp"
#include "lua_usertype_setup.hpp"

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

	lua_usertype_setup(m_game, m_lua_state, this);

	m_lua_state.script_file(script_path, sol::load_mode::text);

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
	if (m_lua_loaded) {
		sol::protected_function_result result = m_lua_process(delta);
		if (!result.valid()) {
			sol::error error = result;
			printf("[LUA_ERROR]: %s\n", error.what());
		}
	}

	for (auto& [key, val] : m_children) {val->Process(delta);}
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::string GameObject::GetName() {return m_name;}
std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
Transform GameObject::GetGlobalTransform() {
	glm::mat4 model_matrix = m_transform.GetModelMatrix();
	GameObject* parent = m_parent;
	while (parent != nullptr) {
		glm::mat4 parent_matrix = parent->GetTransform().GetModelMatrix();
		model_matrix = parent_matrix * model_matrix;
		parent = parent->GetParent();
	}
	vec3 position;
	vec3 scale;
	vec3 skew;
	glm::vec4 perspective;
	glm::quat orientation;
	glm::decompose(model_matrix, scale, orientation, position, skew, perspective);
	vec3 rotation = glm::eulerAngles(orientation);	
	return Transform{position, rotation, scale};
}

GameObject* GameObject::GetParent() {return m_parent;}
GameObject* GameObject::GetChild(std::string name) {
	if (m_children.contains(name)) return m_children[name];
	return nullptr;
}
std::map<std::string, GameObject*> GameObject::GetChildren() {return m_children;}
sol::as_table_t<std::map<std::string,GameObject*>> GameObject::GetChildren_Lua() {
	return (sol::as_table_t<std::map<std::string, GameObject*>>)(m_children);
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
