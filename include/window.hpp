#pragma once
#include <SDL.h>
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
	SDL_Renderer* m_renderer;
};
