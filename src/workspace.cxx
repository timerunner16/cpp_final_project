#include <SDL2/SDL.h>
#include <cmath>
#include "workspace.hpp"

Workspace::Workspace(Game* game) {
	m_game = game;

	m_camera = new Camera(m_game, Transform{glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)}, glm::radians(45.0f));
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

Camera* Workspace::GetCamera() {
	return m_camera;
}

void Workspace::Process() {
	m_camera->GetTransform().position.y = -sin(SDL_GetTicks()/1000.0f)*0.5f;
	for (auto& [key, val] : m_game_objects) {
		val->Process();
	}
}
