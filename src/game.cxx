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

	Mesh* mesh = new Mesh("assets/suzanne.obj");
	Shader* shader = new Shader("assets/basic_vertex.glsl", "assets/basic_frag.glsl");
	
	/*m_workspace->CreateGameObject(std::string("Suzanne"), new GameObject{
		"assets/test.lua",
		new Mesh(std::string("assets/suzanne.obj")),
		new Shader(std::string("assets/basic_vertex.glsl"), std::string("assets/basic_frag.glsl")),
		m_resource_manager->GetGLTexture(std::string("assets/test.png")),
		Transform{glm::vec3(-1.5f, 0.0f, 0.0f),glm::vec3(0.0f),glm::vec3(1.0f)}
	});

	m_workspace->CreateGameObject(std::string("Cube"), new GameObject{
		"",
		new Mesh(std::string("assets/cube.obj")),
		new Shader(std::string("assets/basic_vertex.glsl"), std::string("assets/basic_frag.glsl")),
		m_resource_manager->GetGLTexture(std::string("assets/test.png")),
		Transform{glm::vec3(1.5f, 0.0f, 0.0f),glm::vec3(0.0f),glm::vec3(1.0f)}
	});*/

	for (int i = 0; i < 100; i++) {
		m_workspace->CreateGameObject(std::string("testobj_") + std::to_string(i), new GameObject{
			"",
			mesh,
			shader,
			m_resource_manager->GetGLTexture("assets/test.png"),
			Transform{glm::ballRand(10.0f), glm::vec3(glm::linearRand(0.0f,(float)M_PI*2.0f), glm::linearRand(0.0f,(float)M_PI*2.0f), glm::linearRand(0.0f,(float)M_PI*2.0f)), glm::vec3(glm::linearRand(0.1f,1.5f))}
		});
	}

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
	m_window->Present();
}

Window* Game::GetWindow() {
	return m_window;
}
