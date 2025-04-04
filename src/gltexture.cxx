#include "gltexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GLTexture::GLTexture(std::string texture_path) {
	m_type_str = typeid(GLTexture).name();

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
}

void GLTexture::Cleanup() {
	glDeleteTextures(1, &m_texture_id);
}

GLuint GLTexture::GetTextureID() {return m_texture_id;}

