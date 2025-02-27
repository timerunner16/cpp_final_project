#pragma once
#include <vector>
#include "game_object.hpp"
class Game;

class Workspace {
public:
	Workspace(Game* game);
	~Workspace();
	
	void CreateGameObject(GameObject game_object);
	void Process();
private:
	Game* m_game;
	std::vector<GameObject> m_game_objects;
};
