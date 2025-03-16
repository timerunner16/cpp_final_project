#pragma once
#include <SDL2/SDL.h>
#include "material.hpp"

class Window;
class Workspace;
class ResourceManager;
class Shader;

class Game {
public:
	Game();
	~Game();

	void Process();
	void Render();
	
	Window* GetWindow();
	std::map<std::string, Uniform>* GetGlobalUniforms();
private:
	Window* m_window;
	Workspace* m_workspace;
	ResourceManager* m_resource_manager;
	Material* m_pp_material;

	std::map<std::string, Uniform> m_global_uniforms;

	bool m_should_shutdown;
};
