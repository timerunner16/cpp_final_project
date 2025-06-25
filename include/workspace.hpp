#pragma once
#include <string>
#include <map>
#include <sol/sol.hpp>
#include "camera.hpp"
#include "particle_system.hpp"

class Game;
class GameObject;
class Event;
class Mesh;
class Material;
struct Box;
class AudioInstance;

class Workspace {
public:
	Workspace(Game* game);
	~Workspace();
	
	GameObject* CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::optional<std::shared_ptr<Mesh>> opt_mesh, std::optional<std::shared_ptr<Material>> opt_material,
		const Transform& transform, const vec2& box_bounds, const float& height);
	void CreateParticleSystem(particle_system_create_info info);
	Event* CreateEvent(std::string name, GameObject* parent);
	AudioInstance* CreateAudioInstance(std::string name, std::string filename, GameObject* parent);

	std::map<std::string, GameObject*> GetGameObjects();
	sol::as_table_t<std::map<std::string, GameObject*>> GetGameObjects_Lua();
	GameObject* GetGameObject(std::string name);
	
	std::vector<ParticleSystem*> GetParticleSystems();
	
	AudioInstance* GetAudioInstance(std::string name);
	
	std::map<std::string, Event*> GetEvents();

	Camera* GetCamera();
	
	void Process(float delta);
	void QueueFreeAll();
	void FreeAll();
private:
	Game* m_game;
	std::map<std::string, GameObject*> m_game_objects;
	std::vector<ParticleSystem*> m_particle_systems; 
	std::map<std::string, Event*> m_events;
	std::map<std::string, AudioInstance*> m_audio_instances;
	Camera* m_camera;
	bool m_queued_to_free_all;
};
