#pragma once
#include <SDL2/SDL.h>
#include <chrono>
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

	float GetDelta();
	
	Window* GetWindow();
	Workspace* GetWorkspace();
	ResourceManager* GetResourceManager();
	std::map<std::string, Uniform>* GetGlobalUniforms();
private:
	Window* m_window;
	Workspace* m_workspace;
	ResourceManager* m_resource_manager;
	
	std::shared_ptr<Material> m_pp_material;

	std::map<std::string, Uniform> m_global_uniforms;

	bool m_should_shutdown;

	float m_delta;
	std::chrono::high_resolution_clock::time_point m_tp_a;
	std::chrono::high_resolution_clock::time_point m_tp_b;
};
