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

std::map<std::string, GameObject*> Workspace::GetGameObjects() {return m_game_objects;}

GameObject* Workspace::GetGameObject(std::string name) {
	if (m_game_objects.contains(name)) return m_game_objects[name];
	else return nullptr;
}

Camera* Workspace::GetCamera() {return m_camera;}

void Workspace::Process() {
	m_camera->GetTransform().position.x = sin(SDL_GetTicks()/1000.0f)*5.0f;
	m_camera->GetTransform().position.z =-cos(SDL_GetTicks()/1000.0f)*5.0f;
	m_camera->GetTransform().rotation.y = fmod(SDL_GetTicks()/1000.0f,M_PI*2.0f);
	for (auto& [key, val] : m_game_objects) {
		val->Process();
	}
}
