#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "camera.hpp"

class Game;
class GameObject;
class Material;

class Window {
public:
	Window(Game* game, int width = 640, int height = 480, int downscale = 2);
	~Window();

	void Clear();
	void DrawGameObject(Camera* camera, GameObject* game_object);
	void Present(Material* pp_material);

	int GetWidth();
	int GetHeight();
	int GetDownscale();
private:
	Game* m_game;
	SDL_Window* m_window;
	SDL_GLContext m_context;
	GLuint m_framebuffer, m_color, m_depth;
	GLuint m_pp_vao;

	int m_width, m_height, m_downscale;
};
