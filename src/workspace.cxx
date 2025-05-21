#include <SDL2/SDL.h>
#include <cmath>
#include "workspace.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "event.hpp"

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

void Workspace::CreateParticleSystem(particle_system_create_info info) {
	m_particle_systems.push_back(new ParticleSystem(info));
}

Event* Workspace::CreateEvent(std::string name, GameObject* parent) {
	Event* event = new Event();
	if (parent == nullptr) m_events.insert(std::make_pair(name, event));
	parent->AddEvent(event, name);
	return event;
}

std::map<std::string, GameObject*> Workspace::GetGameObjects() {return m_game_objects;}
std::vector<ParticleSystem*> Workspace::GetParticleSystems() {return m_particle_systems;}
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
	for (auto& i : m_particle_systems) {
		i->Update(delta);
	}
	std::erase_if(m_particle_systems, [](const auto& particle_system) -> bool {
		return particle_system->GetDead();
	});
}
