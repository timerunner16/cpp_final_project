#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

class Game;

class Window {
public:
	Window(Game* game, int width = 640, int height = 480);
	~Window();

	void Clear();
	void Present();
private:
	Game* m_game;
	SDL_Window* m_window;
	SDL_GLContext m_context;
};
