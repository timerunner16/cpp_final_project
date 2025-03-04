#pragma once
#include "GL/glew.h"
#include <string>

struct vertex {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class Mesh {
public:
	Mesh(std::string path);
	~Mesh();

	void GetVertexBufferObject(GLuint& vertex_buffer_object);
	void GetIndexBufferObject(GLuint& index_buffer_object, GLuint& num_indices);
private:
	GLuint m_vertex_buffer_object;
	GLuint m_index_buffer_object;
	GLuint m_num_indices;
};
