#include "window.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <glm/common.hpp>

Window::Window(Game* game, int width, int height, int downscale) {
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

	glGenFramebuffers(1, &m_framebuffer);
	glGenTextures(1, &m_color);
	glGenRenderbuffers(1, &m_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glBindTexture(GL_TEXTURE_2D, m_color);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		width/downscale, height/downscale,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width/downscale, height/downscale);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0,0, width/downscale, height/downscale);

	glGenVertexArrays(1, &m_pp_vao);
	glBindVertexArray(m_pp_vao);

	glm::vec4 vertices[6] = {
		{-1.0f,-1.0f,  0.0f, 0.0f},
		{ 1.0f,-1.0f,  1.0f, 0.0f},
		{ 1.0f, 1.0f,  1.0f, 1.0f},
		{-1.0f,-1.0f,  0.0f, 0.0f},
		{ 1.0f, 1.0f,  1.0f, 1.0f},
		{-1.0f, 1.0f,  0.0f, 1.0f},
	};
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	m_width = width;
	m_height = height;
	m_downscale = downscale;
}

Window::~Window() {
	SDL_DestroyWindow(m_window);
	m_game = nullptr;
	m_window = nullptr;
}

void Window::Clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::DrawGameObject(Camera* camera, GameObject* game_object) {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	
	std::shared_ptr<Mesh> mesh = game_object->GetMesh();
	std::shared_ptr<Shader> shader = game_object->GetShader();
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

void Window::Present(Shader* post_process) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,m_width,m_height);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (post_process == NULL) {
		glBlitNamedFramebuffer(m_framebuffer, 0,
			0, 0, m_width/m_downscale, m_height/m_downscale,
			0, 0, m_width, m_height,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
		);
	} else {
		glBindVertexArray(m_pp_vao);
		
		glUseProgram(post_process->GetProgramID());
		
		post_process->UniformIVec2("window_resolution", glm::ivec2(m_width, m_height));
		post_process->UniformInt("window_downscale", m_downscale);
		
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, m_color);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
		glUseProgram(0);
	}
	SDL_GL_SwapWindow(m_window);
}

int Window::GetWidth() {
	return m_width;
}

int Window::GetHeight() {
	return m_height;
}
