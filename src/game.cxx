#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "resource_manager.hpp"
#include "input.hpp"
#include "vec3.hpp"
#include "map.hpp"
#include "physics.hpp"

Game::Game(std::string wad_path, int width, int height, int downscale, bool resizable) {
	m_wad_path = wad_path;
	m_window = new Window(this, width, height, downscale, resizable);
	m_workspace = new Workspace(this);
	m_resource_manager = new ResourceManager(this);
	m_input_manager = new InputManager(this);
	m_map = new Map(this, "MAP01");
	m_should_shutdown = false;

	m_pp_material = m_resource_manager->GetMaterial("PPDITHER");

	GameObject* root = m_workspace->CreateGameObject(
		"root", nullptr,
		"", nullptr, nullptr,
		Transform(), Box{}
	);
	
	GameObject* observer = m_workspace->CreateGameObject(
		"Observer", root,
		"assets/observer.lua", nullptr, nullptr,
		Transform{vec3(0.0f, 0.0f, -3.0f), vec3(0.0f), vec3(1.0f)}, Box{vec2{0.5f, 0.5f}}
	);

	GameObject* suzanne = m_workspace->CreateGameObject(
		"Suzanne", root,
		"assets/suzanne.lua", m_resource_manager->GetMesh("SUZANNE"), m_resource_manager->GetMaterial("TEST0"),
		Transform{vec3(0.0f, 1.0f, 0.0f), vec3(0.0f), vec3(1.0f)}, Box{vec2{1,1}}
	);

	GameObject* particle_host = m_workspace->CreateGameObject(
		"ParticleHost", root,
		"assets/particle_host.lua", nullptr, nullptr,
		Transform{}, Box{}
	);

	GameObject* particle_creator = m_workspace->CreateGameObject(
		"ParticleCreator", root,
		"assets/particle_creator.lua", nullptr, nullptr,
		Transform{}, Box{}
	);

	m_resource_manager->GetMaterial("TEST0")->SetUniform(Uniform{"snap_scale", INT, (void *)new int{4}});
	m_resource_manager->GetMaterial("TEST1")->SetUniform(Uniform{"snap_scale", INT, (void *)new int{4}});

	m_global_uniforms["window_resolution"] = Uniform{"window_resolution", IVEC2, (void*)new glm::ivec2{m_window->GetWidth(), m_window->GetHeight()}};
	m_global_uniforms["window_downscale"] = Uniform{"window_downscale", INT, (void *)new int{m_window->GetDownscale()}};

	m_tp_a = std::chrono::high_resolution_clock::now();
	m_tp_b = std::chrono::high_resolution_clock::now();
	m_delta = 0.0f;

	while (!m_should_shutdown) {
		Process();
		Render();
	}
}

Game::~Game() {
	delete m_window;
	delete m_workspace;
	delete m_resource_manager;
	delete m_input_manager;
	delete m_map;
	m_window = nullptr;
	m_workspace = nullptr;
	m_resource_manager = nullptr;
	m_input_manager = nullptr;
	m_map = nullptr;
}

void Game::Process() {
	m_tp_a = std::chrono::high_resolution_clock::now();
	m_delta = (m_tp_a-m_tp_b).count()/(1e9);
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case (SDL_QUIT): {
				m_should_shutdown = true;
				break;
			}
			case (SDL_WINDOWEVENT): {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					m_window->Resize(event.window.data1, event.window.data2, m_window->GetDownscale());
				}
				break;
			}
		}
	}
	m_window->Clear();

	m_input_manager->Update();
	m_workspace->Process(m_delta);
	m_resource_manager->ClearUnusedResources();

	m_tp_b = m_tp_a;
}

void Game::Render() {
	m_window->DrawMap(m_workspace->GetCamera());
	for (auto& [key, val] : m_workspace->GetGameObjects()) {
		m_window->DrawGameObject(m_workspace->GetCamera(), val);
	}
	for (const auto& i : m_workspace->GetParticleSystems()) {
		m_window->DrawParticleSystem(m_workspace->GetCamera(), i);
	}
	m_window->Present(m_pp_material);
}

float Game::GetDelta() {
	return m_delta;
}

Window* Game::GetWindow() {return m_window;}
Workspace* Game::GetWorkspace() {return m_workspace;}
ResourceManager* Game::GetResourceManager() {return m_resource_manager;}
InputManager* Game::GetInputManager() {return m_input_manager;}
Map* Game::GetMap() {return m_map;}
std::map<std::string, Uniform>* Game::GetGlobalUniforms() {
	return &m_global_uniforms;
}
std::string Game::GetWADPath() {return m_wad_path;}
