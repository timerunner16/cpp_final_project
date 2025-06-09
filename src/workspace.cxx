#include <SDL2/SDL.h>
#include <cmath>
#include "workspace.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "event.hpp"

Workspace::Workspace(Game* game) {
	m_game = game;

	m_camera = new Camera(m_game, Transform{vec3(0.0f), vec3(0.0f), vec3(1.0f)}, glm::radians(45.0f));
	m_queued_to_free_all = false;
}

Workspace::~Workspace() {
	m_game = nullptr;
}

GameObject* Workspace::CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform, const vec2& box_bounds, const float& height) {
	GameObject* game_object = new GameObject(
		m_game, name, parent,
		script_path, mesh, material,
		transform, Box{box_bounds, vec2(transform.position.x, transform.position.z)}, height
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
	if (m_queued_to_free_all) {FreeAll(); return;}

	for (auto& [key, val] : m_game_objects) {
		val->Process(delta);
	}
	for (auto& i : m_particle_systems) {
		i->Update(delta);
	}
	std::erase_if(m_game_objects, [](const auto& item) -> bool {
		const auto& [key, val] = item;
		if (val->IsQueuedForFreedom()) {
			delete val;
			return true;
		}
		return false;
	});
	std::erase_if(m_particle_systems, [](const auto& particle_system) -> bool {
		return particle_system->GetDead();
	});
}

void Workspace::QueueFreeAll() {
	m_queued_to_free_all = true;
}

void Workspace::FreeAll() {
	m_queued_to_free_all = false;
	for (auto& [key, val] : m_game_objects) {
		delete val;
	}
	m_game_objects.clear();
	for (auto& i : m_particle_systems) {
		delete i;
	}
	m_particle_systems.clear();
	for (auto& [key, val] : m_events) {
		delete val;
	}
	m_events.clear();
}
