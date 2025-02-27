#pragma once
#include <SDL2/SDL.h>
#include "window.hpp"
#include "workspace.hpp"

class Game {
public:
	Game();
	~Game();

	void Process();
	void Render();
private:
	Window* m_window;
	Workspace* m_workspace;

	bool m_should_shutdown;
};
