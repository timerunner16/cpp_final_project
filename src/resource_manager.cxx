#include "resource_manager.hpp"
#include <utility>

GLTexture::GLTexture(std::string texture_path, bool persistent) {
	SDL_Surface* surface = IMG_Load(texture_path.c_str());
	SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexImage2D(
		GL_TEXTURE_2D, 0, 4,
		surface->w, surface->h,
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		surface->pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);

	m_persistent = persistent;
}

void GLTexture::SetPersistent(bool persistent) {m_persistent = persistent;}

GLuint GLTexture::GetTextureID() {return m_texture_id;}
bool GLTexture::GetPersistent() {return m_persistent;}

ResourceManager::ResourceManager() {
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	m_gl_texture_map = std::map<std::string, std::shared_ptr<GLTexture>>();
}

ResourceManager::~ResourceManager() {
	for (auto it = m_gl_texture_map.begin(); it != m_gl_texture_map.end(); it++) {
		DeleteGLTexture(it->first);
	}
}

std::shared_ptr<GLTexture> ResourceManager::GetGLTexture(std::string texture_path) {
	if (m_gl_texture_map.contains(texture_path)) return m_gl_texture_map[texture_path];
	
	GLTexture* gl_texture = new GLTexture{texture_path, false};
	m_gl_texture_map.emplace(std::make_pair(texture_path, gl_texture));
	return m_gl_texture_map[texture_path];
}

void ResourceManager::ClearUnusedGLTextures() {
	for (auto it = m_gl_texture_map.begin(); it != m_gl_texture_map.end(); it++) {
		if (it->second.unique() && !it->second->GetPersistent()) {
			DeleteGLTexture(it->first);
		}
	}
}

void ResourceManager::DeleteGLTexture(std::string texture_path) {
	if (!m_gl_texture_map.contains(texture_path)) return;
	
	GLuint texture_id = m_gl_texture_map[texture_path]->GetTextureID();
	glDeleteTextures(1, &texture_id);
	m_gl_texture_map.erase(texture_path);
}
