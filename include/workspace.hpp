#pragma once
#include <string>
#include <map>
#include "game_object.hpp"
#include "camera.hpp"
#include "particle_system.hpp"
class Game;

class Workspace {
public:
	Workspace(Game* game);
	~Workspace();
	
	GameObject* CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform);
	void CreateParticleSystem(particle_system_create_info info);

	std::map<std::string, GameObject*> GetGameObjects();
	std::vector<ParticleSystem*> GetParticleSystems();
	GameObject* GetGameObject(std::string name);
	sol::as_table_t<std::map<std::string, GameObject*>> GetGameObjects_Lua();
	Camera* GetCamera();
	
	void Process(float delta);
private:
	Game* m_game;
	std::map<std::string, GameObject*> m_game_objects;
	std::vector<ParticleSystem*> m_particle_systems; 
	Camera* m_camera;
};
