#include <SDL2/SDL.h>

class Game {
public:
	Game();
	~Game();

	void Process();
	void Render();
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	bool m_should_shutdown;
};
