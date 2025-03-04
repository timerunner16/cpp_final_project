#include <fstream>
#include <iterator>
#include <list>
#include <vector>
#include "mesh.hpp"
#include <glm/ext.hpp>

enum value_types {
	UNDEFINED,
	VERTEX,
	VERTEX_NORMAL,
	VERTEX_TEXTURE_COORDINATE,
	FACE
};

std::vector<std::string> split_string(std::string input, std::string delimiter) {
	std::vector<std::string> output = std::vector<std::string>();
	std::string remaining_string = input;
	int pos = remaining_string.find(delimiter);
	while (pos != std::string::npos) {
		std::string current = remaining_string.substr(0, pos);
		output.push_back(current);
		remaining_string.erase(0,pos+delimiter.size());
		pos = remaining_string.find(delimiter);
	}
	output.push_back(remaining_string);
	return output;
}

Mesh::Mesh(std::string mesh_path) {
	std::list<glm::vec3> vertex_positions = std::list<glm::vec3>();
	std::list<GLuint> indices = std::list<GLuint>();

	std::ifstream mesh_source_stream;
	mesh_source_stream.open(mesh_path.c_str());

	std::string line;
	while (std::getline(mesh_source_stream, line)) {
		std::vector<std::string> values = split_string(line, " ");
		value_types value_type(UNDEFINED);

		if (values[0] == "v") value_type = VERTEX; 
		else if (values[0] == "vt") value_type = VERTEX_TEXTURE_COORDINATE; 
		else if (values[0] == "vn") value_type = VERTEX_NORMAL; 
		else if (values[0] == "f") value_type = FACE;

		values.erase(values.begin());

		switch (value_type) {
			case (VERTEX): {
				if (values.size() != 3) {printf("OBJ loader only supports three dimensional vertices. Error in file %s with line %s.\n", mesh_path.c_str(), line.c_str()); break;}
				float x,y,z;
				x = atof(values[0].c_str());
				y = atof(values[1].c_str());
				z = atof(values[2].c_str());
				vertex_positions.emplace_back(x,y,z);
				break;
			}
			case (VERTEX_TEXTURE_COORDINATE): {
				break;
			}
			case (VERTEX_NORMAL): {
				break;
			}
			case (FACE): {
				if (values.size() != 3) {printf("OBJ loader only supports triangles. Error in file %s with line %s.\n", mesh_path.c_str(), line.c_str()); break;}
				indices.push_back((GLuint)atoi(values[0].substr(0,values[0].find('/')).c_str()));
				indices.push_back((GLuint)atoi(values[1].substr(0,values[1].find('/')).c_str()));
				indices.push_back((GLuint)atoi(values[2].substr(0,values[2].find('/')).c_str()));
				break;
			}
			case (UNDEFINED): {
				break;
			}
			default: {
				break;
			}
		}
	}
	
	vertex* vertex_data = new vertex[vertex_positions.size()];
	GLuint* index_data = new GLuint[indices.size()];
	int num_vertices = vertex_positions.size();
	m_num_indices = indices.size();

	for (int i = 0; i < vertex_positions.size(); i++) {
		auto vertex_position_iter = vertex_positions.begin();
		std::advance(vertex_position_iter, i);
		vertex_data[i] = vertex{vertex_position_iter->x, vertex_position_iter->y, vertex_position_iter->z};
	}

	for (int i = 0; i < indices.size(); i++) {
		auto index_iter = indices.begin();
		std::advance(index_iter, i);
		index_data[i] = *index_iter;
	}

	glGenBuffers(1, &m_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, num_vertices*sizeof(vertex), vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_num_indices*sizeof(GLuint), index_data, GL_STATIC_DRAW);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &m_vertex_buffer_object);
	glDeleteBuffers(1, &m_index_buffer_object);
}

void Mesh::GetVertexBufferObject(GLuint& vertex_buffer_object) {
	vertex_buffer_object = m_vertex_buffer_object;
}

void Mesh::GetIndexBufferObject(GLuint& index_buffer_object, GLuint& num_indices) {
	index_buffer_object = m_index_buffer_object;
	num_indices = m_num_indices;
}
