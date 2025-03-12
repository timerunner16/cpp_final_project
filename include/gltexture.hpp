#include "resource_manager.hpp"
#include <string>
#include <GL/glew.h>
#include <GL/glu.h>

class GLTexture : public GenericResource {
public:
	GLTexture(std::string file_path);
	
	void Cleanup();

	GLuint GetTextureID();
private:
	GLuint m_texture_id;
};

