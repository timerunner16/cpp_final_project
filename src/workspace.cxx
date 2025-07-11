#include <SDL2/SDL.h>
#include <cmath>
#include "workspace.hpp"
#define AL_LIBTYPE_STATIC
#include <AL/alc.h>
#include "game.hpp"
#include "game_object.hpp"
#include "audio_instance.hpp"
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
		std::string script_path, std::optional<std::shared_ptr<Mesh>> opt_mesh, std::optional<std::shared_ptr<Material>> opt_material,
		const Transform& transform, const vec2& box_bounds, const float& height) {
	std::shared_ptr<Mesh> mesh = nullptr;
	std::shared_ptr<Material> material = nullptr;
	if (opt_mesh.has_value()) mesh = opt_mesh.value();
	if (opt_material.has_value()) material = opt_material.value();
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

AudioInstance* Workspace::CreateAudioInstance(std::string name, std::string filename, GameObject* parent) {
	std::shared_ptr<AudioSegment> audio_segment = m_game->GetResourceManager()->GetAudioSegment(filename);
	AudioInstance* audio_instance = new AudioInstance(audio_segment, parent, m_game);
	if (parent == nullptr) m_audio_instances.insert(std::make_pair(name, audio_instance));
	else parent->AddAudioInstance(audio_instance, name);
	return audio_instance;
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

AudioInstance* Workspace::GetAudioInstance(std::string name) {
	if (m_audio_instances.contains(name)) return m_audio_instances[name];
	return nullptr;
}

Camera* Workspace::GetCamera() {return m_camera;}

void Workspace::Process(float delta) {
	if (m_queued_to_free_all) {FreeAll(); return;}

	alListener3f(AL_POSITION,
		m_camera->GetTransform().position.x,
		m_camera->GetTransform().position.y,
		m_camera->GetTransform().position.z
	);
	vec3 lv = m_camera->GetTransform().GetLookVector();
	vec3 rv = m_camera->GetTransform().GetRightVector();
	float orientation[] = {
		lv.x, lv.y, lv.z,
		rv.x, rv.y, rv.z
	};
	alListenerfv(AL_ORIENTATION, orientation);

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
	for (auto& [key, val] : m_audio_instances) {
		delete val;
	}
	m_audio_instances.clear();
}
