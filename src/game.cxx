#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "resource_manager.hpp"
#include "mesh.hpp"
#include "shader.hpp"

Game::Game() {
	m_window = new Window(this, 1920, 1080);
	m_workspace = new Workspace(this);
	m_resource_manager = new ResourceManager();
	m_should_shutdown = false;

	m_workspace->CreateGameObject(std::string("TestObject"), new GameObject{
		"assets/test.lua",
		new Mesh(std::string("assets/test.obj")),
		new Shader(std::string("assets/basic_vertex.glsl"), std::string("assets/basic_frag.glsl")),
		m_resource_manager->GetGLTexture(std::string("assets/test.png")),
		Transform{glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(1.0f)}
	});

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
	m_window->DrawGameObject(m_workspace->GetCamera(), m_workspace->GetGameObject(std::string("TestObject")));
	m_window->Present();
}

Window* Game::GetWindow() {
	return m_window;
}
