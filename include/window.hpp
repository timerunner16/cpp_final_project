#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "camera.hpp"

class Game;
class GameObject;

class Window {
public:
	Window(Game* game, int width = 640, int height = 480);
	~Window();

	void Clear();
	void DrawGameObject(Camera* camera, GameObject* game_object);
	void Present();

	int GetWidth();
	int GetHeight();
private:
	Game* m_game;
	SDL_Window* m_window;
	SDL_GLContext m_context;

	int m_width, m_height;
};
