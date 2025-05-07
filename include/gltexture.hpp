#include "resource_manager.hpp"
#include <GL/glew.h>
#include <GL/glu.h>

class GLTexture : public GenericResource {
public:
	GLTexture(uint8_t* data, uint32_t size);
	
	void Cleanup();

	GLuint GetTextureID();
	int GetWidth();
	int GetHeight();
private:
	GLuint m_texture_id;
	int m_width;
	int m_height;
};

