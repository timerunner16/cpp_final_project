#pragma once
#include <string>
#include <map>
#include "game_object.hpp"
#include "camera.hpp"
class Game;

class Workspace {
public:
	Workspace(Game* game);
	~Workspace();
	
	GameObject* CreateGameObject(std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform);

	std::map<std::string, GameObject*> GetGameObjects();
	GameObject* GetGameObject(std::string name);
	sol::as_table_t<std::map<std::string, GameObject*>> GetGameObjects_Lua();
	Camera* GetCamera();
	
	void Process(float delta);
private:
	Game* m_game;
	std::map<std::string, GameObject*> m_game_objects;
	Camera* m_camera;
};
