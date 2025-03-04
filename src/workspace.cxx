#include "workspace.hpp"

Workspace::Workspace(Game* game) {
	m_game = game;
}

Workspace::~Workspace() {
	m_game = nullptr;
}

void Workspace::CreateGameObject(std::string name, GameObject* game_object) {
	m_game_objects.insert(std::make_pair(name, game_object));
}

GameObject* Workspace::GetGameObject(std::string name) {
	if (m_game_objects.contains(name)) return m_game_objects[name];
	else return nullptr;
}

void Workspace::Process() {
	for (auto& [key, val] : m_game_objects) {
		val->Process();
	}
}
