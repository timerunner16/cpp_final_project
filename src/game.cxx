#include "game.hpp"
#include "window.hpp"

Game::Game() {
	m_window = new Window(this, 640, 480);
	m_workspace = new Workspace();
	m_should_shutdown = false;

	m_workspace->Test();
	delete m_workspace;
	m_workspace = nullptr;

	while (!m_should_shutdown) {
		Process();
		Render();
	}
}

Game::~Game() {
	delete m_window;
	m_window = nullptr;
}

void Game::Process() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) m_should_shutdown = true;
	}
}

void Game::Render() {
	m_window->Clear();
	m_window->Present();
}
