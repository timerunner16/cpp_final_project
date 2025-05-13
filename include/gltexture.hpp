#include "resource_manager.hpp"
#include <GL/glew.h>
#include <GL/glu.h>

enum IMAGE_FORMAT {
	PNG,
	BMP,
	JPG
};

class GLTexture : public GenericResource {
public:
	GLTexture(uint8_t* data, uint32_t size, IMAGE_FORMAT format = PNG);
	
	void Cleanup();

	GLuint GetTextureID();
	int GetWidth();
	int GetHeight();
private:
	GLuint m_texture_id;
	int m_width;
	int m_height;
};

