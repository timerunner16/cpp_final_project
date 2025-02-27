#include "window.hpp"
#include "game.hpp"

Window::Window(Game* game, int width, int height) {
	m_game = game;
	m_window = SDL_CreateWindow("Window", 0, 0, width, height, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_ACCELERATED);
}

Window::~Window() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	m_game = nullptr;
	m_window = nullptr;
	m_renderer = nullptr;
}

void Window::Clear() {
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
}

void Window::Present() {
	SDL_RenderPresent(m_renderer);
}
