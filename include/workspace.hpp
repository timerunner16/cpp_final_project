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
class Box;

class Workspace {
public:
	Workspace(Game* game);
	~Workspace();
	
	GameObject* CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform, const vec2& box_bounds, const float& height);
	void CreateParticleSystem(particle_system_create_info info);
	Event* CreateEvent(std::string name, GameObject* parent);

	std::map<std::string, GameObject*> GetGameObjects();
	std::vector<ParticleSystem*> GetParticleSystems();
	GameObject* GetGameObject(std::string name);
	sol::as_table_t<std::map<std::string, GameObject*>> GetGameObjects_Lua();
	Camera* GetCamera();
	
	void Process(float delta);
	void QueueFreeAll();
	void FreeAll();
private:
	Game* m_game;
	std::map<std::string, GameObject*> m_game_objects;
	std::vector<ParticleSystem*> m_particle_systems; 
	std::map<std::string, Event*> m_events;
	Camera* m_camera;
	bool m_queued_to_free_all;
};
