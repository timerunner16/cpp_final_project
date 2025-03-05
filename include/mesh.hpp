#pragma once
#include "GL/glew.h"
#include <glm/ext.hpp>
#include <string>

struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
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
