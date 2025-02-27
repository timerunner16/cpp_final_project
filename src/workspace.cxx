#include "workspace.hpp"

Workspace::Workspace(Game* game) {
	m_game = game;
}

Workspace::~Workspace() {
	m_game = nullptr;
}

void Workspace::CreateGameObject(GameObject game_object) {
	m_game_objects.push_back(game_object);
}

void Workspace::Process() {
	for (GameObject game_object : m_game_objects) {
		game_object.Process();
	}
}
