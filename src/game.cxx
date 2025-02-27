#include "game.hpp"

Game::Game() {
	m_window = SDL_CreateWindow("Test", 0, 0, 1280, 960, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_ACCELERATED);
	m_should_shutdown = false;

	while (!m_should_shutdown) {
		Process();
		Render();
	}
}

Game::~Game() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
}

void Game::Process() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) m_should_shutdown = true;
	}
}

void Game::Render() {
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);
	SDL_RenderPresent(m_renderer);
}
