#include "game.hpp"
#include "game_object.hpp"
#include "window.hpp"
#include "workspace.hpp"
#include "mesh.hpp"
#include "shader.hpp"

Game::Game() {
	m_window = new Window(this, 640, 480);
	m_workspace = new Workspace(this);
	m_should_shutdown = false;

	m_workspace->CreateGameObject(std::string("TestObject"), new GameObject{"assets/test.lua"});

	GameObject* game_object = m_workspace->GetGameObject(std::string("TestObject"));
	game_object->SetMesh(new Mesh(std::string("assets/test.obj")));
	game_object->SetShader(new Shader(std::string("assets/basic_vertex.glsl"), std::string("assets/basic_frag.glsl")));

	while (!m_should_shutdown) {
		Process();
		Render();
	}
}

Game::~Game() {
	delete m_window;
	m_window = nullptr;
	delete m_workspace;
	m_workspace = nullptr;
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
	m_window->DrawGameObject(m_workspace->GetGameObject(std::string("TestObject")));
	m_window->Present();
}
