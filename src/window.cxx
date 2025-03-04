#include "window.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "mesh.hpp"
#include "shader.hpp"

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

	m_width = width;
	m_height = height;
}

Window::~Window() {
	SDL_DestroyWindow(m_window);
	m_game = nullptr;
	m_window = nullptr;
}

void Window::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::DrawGameObject(Camera* camera, GameObject* game_object) {
	Mesh* mesh = game_object->GetMesh();
	Shader* shader = game_object->GetShader();
	GLuint vertex_buffer_object, index_buffer_object, num_indices;
	mesh->GetVertexBufferObject(vertex_buffer_object);
	mesh->GetIndexBufferObject(index_buffer_object,num_indices);

	glUseProgram(game_object->GetShader()->GetProgramID());

	game_object->GetShader()->UniformMat4("model", game_object->GetTransform().GetModelMatrix());
	game_object->GetShader()->UniformMat4("view", camera->GetViewMatrix());
	game_object->GetShader()->UniformMat4("projection", camera->GetProjectionMatrix());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(0);
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
