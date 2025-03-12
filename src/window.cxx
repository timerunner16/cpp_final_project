#include "window.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <glm/common.hpp>

Window::Window(Game* game, int width, int height) {
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_game = game;
	m_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	m_context = SDL_GL_CreateContext(m_window);
	glewExperimental = true;
	GLenum glewError = glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_width = width;
	m_height = height;
}

Window::~Window() {
	SDL_DestroyWindow(m_window);
	m_game = nullptr;
	m_window = nullptr;
}

void Window::Clear() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::DrawGameObject(Camera* camera, GameObject* game_object) {
	std::shared_ptr<Mesh> mesh = game_object->GetMesh();
	Shader* shader = game_object->GetShader();
	GLuint vertex_array_object = 0;
	GLuint num_indices = 0;
	mesh->GetVertexArrayObject(vertex_array_object, num_indices);

	glBindVertexArray(vertex_array_object);

	glUseProgram(game_object->GetShader()->GetProgramID());

	glm::mat4 model_matrix = game_object->GetTransform().GetModelMatrix();
	glm::mat4 view_matrix = camera->GetViewMatrix();
	game_object->GetShader()->UniformMat4("model_view_matrix", view_matrix*model_matrix);
	game_object->GetShader()->UniformMat4("normal_matrix", glm::transpose(inverse(model_matrix)));
	game_object->GetShader()->UniformMat4("projection_matrix", camera->GetProjectionMatrix());

	glBindTexture(GL_TEXTURE_2D, game_object->GetGLTexture()->GetTextureID());

	glDrawArrays(GL_TRIANGLES, 0, num_indices);

	glUseProgram(0);
}

void Window::Present() {
	SDL_GL_SwapWindow(m_window);
}

int Window::GetWidth() {
	return m_width;
}

int Window::GetHeight() {
	return m_height;
}
