#include <glm/common.hpp>
#include <vector>
#include "window.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "map.hpp"
#include "particle_system.hpp"
#include "shader.hpp"
#include "builtin_text_shader.h"
#include "builtin_pp_shader.h"
#include "gltexture.hpp"
#include "engine_font.hpp"

//#define MACROPRINT
//#define VERBOSE_DBPRINTF
#include "macroprint.h"

Window::Window(Game* game, int width, int height, int downscale, bool resizable, bool find_size) {
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (find_size) {
		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm);
		int d_width = dm.w;
		int d_height = dm.h;
		if (d_width/4 < d_height/3) {
			downscale = d_width / 320 - 1;
			width = downscale * 320;
			height = (width / 4) * 3;
		} else {
			downscale = d_height / 240 - 1;
			height = downscale * 240;
			width = (height / 3) * 4;
		}
	}

	m_game = game;
	m_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE);

	m_context = SDL_GL_CreateContext(m_window);
	glewExperimental = true;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Resize(width, height, downscale);

	glGenVertexArrays(1, &m_pp_vao);
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
	glBindVertexArray(m_pp_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glGenBuffers(1, &m_text_vbo);

	m_text_shader = new Shader((uint8_t*)shader_source.c_str(), (uint32_t)shader_source.size());

	m_text_texture = new GLTexture((uint8_t*)__engine_font, __engine_font_len, BMP);

	m_default_pp_shader = new Shader((uint8_t*)pp_shader_source.c_str(), (uint32_t)pp_shader_source.size());

	m_width = width;
	m_height = height;
	m_downscale = downscale;
}

Window::~Window() {
	delete m_text_shader;
	delete m_text_texture;
	delete m_default_pp_shader;
	SDL_DestroyWindow(m_window);
	m_game = nullptr;
	m_window = nullptr;
	m_text_shader = nullptr;
	m_text_texture = nullptr;
	m_default_pp_shader = nullptr;
}

void Window::Clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_text_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::DrawGameObject(Camera* camera, GameObject* game_object) {
	if (game_object == nullptr || camera == nullptr) return;
	for (auto& [key, val] : game_object->GetChildren()) {
		DrawGameObject(camera, val);
	}
	
	if (!game_object->GetVisible()) return;
	if (game_object->GetMesh() == nullptr) return;
	if (game_object->GetMaterial() == nullptr) {DBPRINTF("Can't render a mesh without a material!\n"); return;}

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	
	std::shared_ptr<Mesh> mesh = game_object->GetMesh();
	GLuint vertex_array_object = 0;
	GLuint num_indices = 0;
	mesh->GetVertexArrayObject(vertex_array_object, num_indices);

	glBindVertexArray(vertex_array_object);

	glm::mat4 model_matrix = game_object->GetTransform().GetModelMatrix();
	GameObject* parent = game_object->GetParent();
	while (parent != nullptr) {
		glm::mat4 parent_matrix = parent->GetTransform().GetModelMatrix();
		model_matrix = parent_matrix * model_matrix;
		parent = parent->GetParent();
	}
	glm::mat4 view_matrix = camera->GetViewMatrix();
	glm::mat4 model_view_matrix = view_matrix * model_matrix;
	glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_matrix));
	glm::mat4 inv_model_view_matrix = glm::transpose(glm::inverse(model_view_matrix));
	glm::mat4 inv_view_matrix = glm::transpose(glm::inverse(view_matrix));
	glm::mat4 projection_matrix = camera->GetProjectionMatrix();

	Uniform u_model_view_matrix = Uniform{"model_view_matrix", UN_MAT4, &model_view_matrix};
	Uniform u_normal_matrix = Uniform{"normal_matrix", UN_MAT4, &normal_matrix};
	Uniform u_inv_model_view_matrix = Uniform{"inv_model_view_matrix", UN_MAT4, &inv_model_view_matrix};
	Uniform u_inv_view_matrix = Uniform{"inv_view_matrix", UN_MAT4, &inv_view_matrix};
	Uniform u_projection_matrix = Uniform{"projection_matrix", UN_MAT4, &projection_matrix};
	
	game_object->GetMaterial()->SetUniform(u_model_view_matrix);
	game_object->GetMaterial()->SetUniform(u_normal_matrix);
	game_object->GetMaterial()->SetUniform(u_inv_model_view_matrix);
	game_object->GetMaterial()->SetUniform(u_inv_view_matrix);
	game_object->GetMaterial()->SetUniform(u_projection_matrix);

	game_object->GetMaterial()->Bind(m_game);

	for (const auto& [key, val] : game_object->GetUniforms()) {
		game_object->GetMaterial()->ApplyUniform(val);
	}

	if (m_wireframe) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glDrawArrays(GL_TRIANGLES, 0, num_indices);

	if (m_wireframe) {
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Window::DrawMap(Camera* camera) {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);

	std::vector<MapSegmentRenderData> map_segments = m_game->GetMap()->GetMapSegments();

	glm::mat4 view_matrix = camera->GetViewMatrix();
	glm::mat4 model_view_matrix = view_matrix * glm::mat4(1.0f);
	glm::mat4 normal_matrix = glm::transpose(glm::inverse(glm::mat4(1.0f)));
	glm::mat4 inv_model_view_matrix = glm::transpose(glm::inverse(model_view_matrix));
	glm::mat4 inv_view_matrix = glm::transpose(glm::inverse(view_matrix));
	glm::mat4 projection_matrix = camera->GetProjectionMatrix();

	Uniform u_model_view_matrix = Uniform{"model_view_matrix", UN_MAT4, &model_view_matrix};
	Uniform u_normal_matrix = Uniform{"normal_matrix", UN_MAT4, &normal_matrix};
	Uniform u_inv_model_view_matrix = Uniform{"inv_model_view_matrix", UN_MAT4, &inv_model_view_matrix};
	Uniform u_inv_view_matrix = Uniform{"inv_view_matrix", UN_MAT4, &inv_view_matrix};
	Uniform u_projection_matrix = Uniform{"projection_matrix", UN_MAT4, &projection_matrix};
	
	if (m_wireframe) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for (MapSegmentRenderData map_segment : map_segments) {
		if (map_segment.material == nullptr) continue;

		glBindVertexArray(map_segment.vao);

		map_segment.material->SetUniform(u_model_view_matrix);
		map_segment.material->SetUniform(u_normal_matrix);
		map_segment.material->SetUniform(u_inv_model_view_matrix);
		map_segment.material->SetUniform(u_inv_view_matrix);
		map_segment.material->SetUniform(u_projection_matrix);
		
		map_segment.material->Bind(m_game);

		glDrawArrays(GL_TRIANGLES, 0, map_segment.num_indices);
	}

	if (m_wireframe) {
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Window::DrawString(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, std::string string) {
	std::vector<glm::vec4> vertices(0);
	int lastx = x;
	
	for (char c : string) {
		int minx = lastx;
		int miny = y;
		int maxx = minx + 8;
		int maxy = y + 8;
		
		int width_res = m_width/m_downscale;
		int height_res = m_height/m_downscale;

		float c_minx = (2.0f*((float)minx/width_res)-1.0f);
		float c_miny =-(2.0f*((float)miny/height_res)-1.0f);
		float c_maxx = (2.0f*((float)maxx/width_res)-1.0f);
		float c_maxy =-(2.0f*((float)maxy/height_res)-1.0f);

		uint8_t row = ((c&0xf0) >> 4) - 2;
		uint8_t col = c&0x0f;
		if (row > 5) row = 0;

		float t_minx = (float)(col*8)/m_text_texture->GetWidth();
		float t_miny = (float)(row*8)/m_text_texture->GetHeight();
		float t_maxx = (float)(col*8+8)/m_text_texture->GetWidth();
		float t_maxy = (float)(row*8+8)/m_text_texture->GetHeight();
		
		vertices.push_back({c_minx, c_miny, t_minx, t_miny});
		vertices.push_back({c_maxx, c_maxy, t_maxx, t_maxy});
		vertices.push_back({c_maxx, c_miny, t_maxx, t_miny});
		vertices.push_back({c_minx, c_miny, t_minx, t_miny});
		vertices.push_back({c_minx, c_maxy, t_minx, t_maxy});
		vertices.push_back({c_maxx, c_maxy, t_maxx, t_maxy});
		
		lastx += 8;
	}

	glm::vec4 color{(float)r/255, (float)g/255, (float)b/255, (float)a/255};

	glBindFramebuffer(GL_FRAMEBUFFER, m_text_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_text_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), vertices.data(), GL_STATIC_DRAW);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(m_text_shader->GetProgramID());
	m_text_shader->UniformVec4("modulate", color);
	glBindTexture(GL_TEXTURE_2D, m_text_texture->GetTextureID());
	
	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
	
	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}

void Window::DrawParticleSystem(Camera* camera, ParticleSystem* particle_system) {
	if (particle_system == nullptr || camera == nullptr) return;

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0,0,m_width/m_downscale,m_height/m_downscale);

	glBindVertexArray(particle_system->GetVAO());
	
	glm::mat4 view_matrix = camera->GetViewMatrix();
	glm::mat4 projection_matrix = camera->GetProjectionMatrix();
	
	Uniform u_model_view_matrix = Uniform{"view", UN_MAT4, &view_matrix};
	Uniform u_projection_matrix = Uniform{"projection", UN_MAT4, &projection_matrix};
	
	particle_system->GetMaterial()->SetUniform(u_model_view_matrix);
	particle_system->GetMaterial()->SetUniform(u_projection_matrix);

	particle_system->GetMaterial()->Bind(m_game);
	
	if (m_wireframe) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)particle_system->GetInstances());

	if (m_wireframe) {
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Window::Present(std::shared_ptr<Material> pp_material) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,m_width,m_height);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	
	glBindVertexArray(m_pp_vao);
	
	if (pp_material == nullptr) {
		glUseProgram(m_default_pp_shader->GetProgramID());
		
		glBindTexture(GL_TEXTURE_2D, m_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindTexture(GL_TEXTURE_2D, m_text_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	} else {
		pp_material->Bind(m_game);
		
		glBindTexture(GL_TEXTURE_2D, m_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glUseProgram(m_default_pp_shader->GetProgramID());

		glBindTexture(GL_TEXTURE_2D, m_text_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);
	glUseProgram(0);

	SDL_GL_SwapWindow(m_window);
}

void Window::Resize(int width, int height, int downscale) {
	m_width = width;
	m_height = height;
	m_downscale = downscale;
	
	if (m_framebuffer != 0) {
		glDeleteFramebuffers(1, &m_framebuffer);
		glDeleteTextures(1, &m_color);
		glDeleteTextures(1, &m_depth);
	}

	glGenFramebuffers(1, &m_framebuffer);
	glGenTextures(1, &m_color);
	glGenTextures(1, &m_depth);

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

	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width/downscale, height/downscale,
			0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0,0, width/downscale, height/downscale);

	if (m_text_framebuffer != 0) {
		glDeleteFramebuffers(1, &m_text_framebuffer);
		glDeleteTextures(1, &m_text_color);
		glDeleteTextures(1, &m_text_depth);
	}

	glGenFramebuffers(1, &m_text_framebuffer);
	glGenTextures(1, &m_text_color);
	glGenTextures(1, &m_text_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, m_text_framebuffer);
	glBindTexture(GL_TEXTURE_2D, m_text_color);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		width/downscale, height/downscale,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_text_color, 0);

	glBindTexture(GL_TEXTURE_2D, m_text_depth);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width/downscale, height/downscale,
		0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_text_depth, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0,0, width/downscale, height/downscale);
}

void Window::SetWireframeEnabled(bool enabled) {
	m_wireframe = enabled;
}

int Window::GetWidth() {return m_width;}
int Window::GetHeight() {return m_height;}
int Window::GetDownscale() {return m_downscale;}

bool Window::GetFocused() {
	return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}
