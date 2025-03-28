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
	
	void CreateGameObject(std::string name, GameObject* game_object);

	std::map<std::string, GameObject*> GetGameObjects();
	GameObject* GetGameObject(std::string name);
	Camera* GetCamera();
	
	void Process(float delta);
private:
	Game* m_game;
	std::map<std::string, GameObject*> m_game_objects;
	Camera* m_camera;
};
