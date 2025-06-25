#pragma once
#include "glcommon.hpp"
#include <glm/ext.hpp>
#include <string>
#include "resource_manager.hpp"

struct mesh_vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh : public GenericResource {
public:
	Mesh(std::vector<std::string> mesh_source);

	void Cleanup();

	void GetVertexArrayObject(GLuint& vertex_array_object, GLuint& num_indices);
private:
	GLuint m_vertex_array_object;
	GLuint m_vertex_buffer_object;
	GLuint m_index_buffer_object;
	GLuint m_num_indices;
};
