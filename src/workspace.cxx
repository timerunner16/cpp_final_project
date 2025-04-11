#include <SDL2/SDL.h>
#include <cmath>
#include "workspace.hpp"
#include "game.hpp"

Workspace::Workspace(Game* game) {
	m_game = game;

	m_camera = new Camera(m_game, Transform{vec3(0.0f), vec3(0.0f), vec3(1.0f)}, glm::radians(45.0f));
}

Workspace::~Workspace() {
	m_game = nullptr;
}

GameObject* Workspace::CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform) {
	GameObject* game_object = new GameObject(
		m_game, name, parent,
		script_path, mesh, material,
		transform
	);
	if (parent == nullptr) m_game_objects.insert(std::make_pair(name, game_object));
	return game_object;
}

std::map<std::string, GameObject*> Workspace::GetGameObjects() {return m_game_objects;}
sol::as_table_t<std::map<std::string, GameObject*>> Workspace::GetGameObjects_Lua() {
	return (sol::as_table_t<std::map<std::string, GameObject*>>)(m_game_objects);
}

GameObject* Workspace::GetGameObject(std::string name) {
	if (m_game_objects.contains(name)) return m_game_objects[name];
	return nullptr;
}

Camera* Workspace::GetCamera() {return m_camera;}

void Workspace::Process(float delta) {
	for (auto& [key, val] : m_game_objects) {
		val->Process(delta);
	}
}
