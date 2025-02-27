#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"

Game::Game() {
	m_window = new Window(this, 640, 480);
	m_workspace = new Workspace(this);
	m_should_shutdown = false;

	m_workspace->CreateGameObject(GameObject{"assets/test.lua"});

	while (!m_should_shutdown) {
		Process();
		Render();
	}
}

Game::~Game() {
	delete m_window;
	m_window = nullptr;
	delete m_workspace;
	m_workspace = nullptr;
}

void Game::Process() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) m_should_shutdown = true;
	}
	m_workspace->Process();
}

void Game::Render() {
	m_window->Clear();
	m_window->Present();
}
