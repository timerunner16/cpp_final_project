#pragma once
#include <SDL2/SDL.h>
#include <chrono>
#include "material.hpp"

class Window;
class Workspace;
class ResourceManager;
class InputManager;
class PDataManager;
class Map;

class Game {
public:
	Game(std::string wad_path = "assets/testmap.wad", int width = 320, int height = 240, int downscale = 1, bool resizable = false);
	~Game();

	void Process();
	void Render();

	float GetDelta();
	
	Window* GetWindow();
	Workspace* GetWorkspace();
	ResourceManager* GetResourceManager();
	InputManager* GetInputManager();
	PDataManager* GetPDataManager();
	Map* GetMap();
	std::map<std::string, Uniform>* GetGlobalUniforms();
	std::string GetWADPath();
private:
	Window* m_window;
	Workspace* m_workspace;
	ResourceManager* m_resource_manager;
	InputManager* m_input_manager;
	PDataManager* m_pdata_manager;
	Map* m_map;
	std::string m_wad_path;
	
	std::shared_ptr<Material> m_pp_material;

	std::map<std::string, Uniform> m_global_uniforms;

	bool m_should_shutdown;

	float m_delta;
	float m_time;
	std::chrono::high_resolution_clock::time_point m_tp_a;
	std::chrono::high_resolution_clock::time_point m_tp_b;
};
