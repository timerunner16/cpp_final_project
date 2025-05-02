#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <memory>
#include "camera.hpp"

class Game;
class GameObject;
class Material;

class Window {
public:
	Window(Game* game, int width = 640, int height = 480, int downscale = 2, bool resizable = false);
	~Window();

	void Clear();
	void DrawGameObject(Camera* camera, GameObject* game_object);
	void DrawMap(Camera* camera);
	void Present(std::shared_ptr<Material> pp_material);
	void Resize(int width, int height, int downscale);
	void SetWireframeEnabled(bool enabled);

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
	bool m_wireframe;
};
