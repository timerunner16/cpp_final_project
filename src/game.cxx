#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "resource_manager.hpp"
#include "material.hpp"
#include "vec3.hpp"

Game::Game() {
	m_window = new Window(this, 960, 720, 3, true);
	m_workspace = new Workspace(this);
	m_resource_manager = new ResourceManager(this);
	m_should_shutdown = false;

	m_pp_material = m_resource_manager->GetResource<Material>("assets/postprocess.mat");

	std::shared_ptr<Material> testmat = m_resource_manager->GetResource<Material>("assets/test.mat");
	GameObject* root = m_workspace->CreateGameObject(
		"root", nullptr,
		"", nullptr, nullptr,
		Transform()
	);

	GameObject* suzanne = m_workspace->CreateGameObject(
		"suzanne", root,
		"assets/suzanne.lua", m_resource_manager->GetResource<Mesh>("assets/suzanne.obj"), testmat,
		Transform{vec3(0.0f), vec3(0.0f), vec3(1.0f)}
	);
	
	GameObject* signal_test = m_workspace->CreateGameObject(
		"signal_test", suzanne,
		"assets/signal_test.lua", m_resource_manager->GetResource<Mesh>("assets/cube.obj"), testmat,
		Transform{vec3(1.0f,1.0f,0.0f), vec3(0.0f), vec3(0.5f)}
	);

	suzanne->GetMaterial()->SetUniform(Uniform{"snap_scale", INT, (void *)new int{4}});
	signal_test->GetMaterial()->SetUniform(Uniform{"snap_scale", INT, (void *)new int{4}});

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
	m_window = nullptr;
	m_workspace = nullptr;
	m_resource_manager = nullptr;
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
	m_workspace->Process(m_delta);

	m_tp_b = m_tp_a;
}

void Game::Render() {
	m_window->Clear();
	for (auto [key, val] : m_workspace->GetGameObjects()) {
		m_window->DrawGameObject(m_workspace->GetCamera(), val);
	}
	m_window->Present(m_pp_material);
}

float Game::GetDelta() {
	return m_delta;
}

Window* Game::GetWindow() {return m_window;}
Workspace* Game::GetWorkspace() {return m_workspace;}
ResourceManager* Game::GetResourceManager() {return m_resource_manager;}

std::map<std::string, Uniform>* Game::GetGlobalUniforms() {
	return &m_global_uniforms;
}
