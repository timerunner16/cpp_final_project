#include "gltexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rwops.h>

GLTexture::GLTexture(uint8_t* data, uint32_t size, IMAGE_FORMAT format) {
	SDL_RWops* rw = SDL_RWFromConstMem(data, size);
	SDL_Surface* surface = nullptr;
	switch (format) {
		case PNG:
			surface = IMG_LoadPNG_RW(rw);
			break;
		case BMP:
			surface = IMG_LoadBMP_RW(rw);
			break;
		case JPG:
			surface = IMG_LoadJPG_RW(rw);
			break;
	}
	delete rw;
	GLenum texture_format = GL_RGBA;
	GLint num_colors;

	num_colors = surface->format->BytesPerPixel;
	if (num_colors == 4) {
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} else if (num_colors == 3) {
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}
	
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexImage2D(
		GL_TEXTURE_2D, 0, num_colors,
		surface->w, surface->h,
		0, texture_format, GL_UNSIGNED_BYTE,
		surface->pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_width = surface->w;
	m_height = surface->h;

	SDL_FreeSurface(surface);
}
void GLTexture::Cleanup() {
	glDeleteTextures(1, &m_texture_id);
}

GLuint GLTexture::GetTextureID() {return m_texture_id;}

int GLTexture::GetWidth() {return m_width;}
int GLTexture::GetHeight() {return m_height;}
