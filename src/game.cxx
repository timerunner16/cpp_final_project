#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "resource_manager.hpp"
#include "material.hpp"
#include "gltexture.hpp"
#include "shader.hpp"

Game::Game() {
	m_window = new Window(this, 1920, 1440, 6);
	m_workspace = new Workspace(this);
	m_resource_manager = new ResourceManager();
	m_should_shutdown = false;

	m_pp_material = new Material{std::make_shared<Shader>("assets/pp_quantize_dither.glsl"), m_resource_manager->GetResource<GLTexture>("assets/test.png")};

	m_workspace->CreateGameObject("testobj", new GameObject{
		"",
		m_resource_manager->GetResource<Mesh>("assets/suzanne.obj"),
		std::make_shared<Material>(m_resource_manager->GetResource<Shader>("assets/basic.glsl"), m_resource_manager->GetResource<GLTexture>("assets/test.png")),
		Transform{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)},
	});

	m_workspace->GetGameObject("testobj")->GetMaterial()->SetUniform(Uniform{"snap_scale", INT, (void *)new int{4}});

	m_global_uniforms["window_resolution"] = Uniform{"window_resolution", IVEC2, (void*)new glm::ivec2{m_window->GetWidth(), m_window->GetHeight()}};
	m_global_uniforms["window_downscale"] = Uniform{"window_downscale", INT, (void *)new int{m_window->GetDownscale()}};

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
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) m_should_shutdown = true;
	}
	m_workspace->Process();
}

void Game::Render() {
	m_window->Clear();
	for (auto game_object_pair : m_workspace->GetGameObjects()) {
		m_window->DrawGameObject(m_workspace->GetCamera(), game_object_pair.second);
	}
	m_window->Present(m_pp_material);
}

Window* Game::GetWindow() {
	return m_window;
}

std::map<std::string, Uniform>* Game::GetGlobalUniforms() {
	return &m_global_uniforms;
}
