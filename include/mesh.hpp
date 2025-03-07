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

	void GetVertexArrayObject(GLuint& vertex_array_object, GLuint& num_indices);
private:
	GLuint m_vertex_array_object;
	GLuint m_vertex_buffer_object;
	GLuint m_index_buffer_object;
	GLuint m_num_indices;
};
