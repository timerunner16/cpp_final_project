#pragma once
#include <SDL2/SDL.h>

class Window;
class Workspace;

class Game {
public:
	Game();
	~Game();

	void Process();
	void Render();
	
	Window* GetWindow();
private:
	Window* m_window;
	Workspace* m_workspace;

	bool m_should_shutdown;
};
