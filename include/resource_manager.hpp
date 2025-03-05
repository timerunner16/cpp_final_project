#pragma once
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>
#include <memory>

class GLTexture {
public:
	GLTexture(std::string texture_path, bool persistent=false);

	void SetPersistent(bool persistent);

	GLuint GetTextureID();
	bool GetPersistent();
private:
	GLuint m_texture_id;
	bool m_persistent;
};

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void ClearUnusedGLTextures();

	std::shared_ptr<GLTexture> GetGLTexture(std::string texture_path);
private:
	void DeleteGLTexture(std::string texture_path);

	std::map<std::string, std::shared_ptr<GLTexture>> m_gl_texture_map;
};
