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

struct index_group {
	unsigned int position_index;
	unsigned int texture_coordinate_index;
	unsigned int normal_index;
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
	std::list<glm::vec3> vertex_normals = std::list<glm::vec3>();
	std::list<glm::vec2> vertex_texture_coordinates = std::list<glm::vec2>();
	std::list<index_group> indices = std::list<index_group>();

	std::ifstream mesh_source_stream(mesh_path.c_str());
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
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				x = std::stof(values[0]);
				y = std::stof(values[1]);
				z = std::stof(values[2]);
				vertex_positions.push_back(glm::vec3(x,y,z));
				break;
			}
			case (VERTEX_TEXTURE_COORDINATE): {
				if (values.size() != 2) {printf("OBJ loader only supports two dimensional texture coordinates. Error in file %s with line %s.\n", mesh_path.c_str(), line.c_str()); break;}
				float u = 0.0f;
				float v = 0.0f;
				u = stof(values[0]);
				v = stof(values[1]);
				vertex_texture_coordinates.push_back(glm::vec2(u,v));
				break;
			}
			case (VERTEX_NORMAL): {
				if (values.size() != 3) {printf("OBJ loader only supports three dimensional vertices. Error in file %s with line %s.\n", mesh_path.c_str(), line.c_str()); break;}
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;;
				x = stof(values[0]);
				y = stof(values[1]);
				z = stof(values[2]);
				vertex_normals.push_back(glm::vec3(x,y,z));
				break;
			}
			case (FACE): {
				if (values.size() != 3) {printf("OBJ loader only supports triangles. Error in file %s with line %s.\n", mesh_path.c_str(), line.c_str()); break;}
				std::vector<std::string> index_group_a = split_string(values[0], "/");
				std::vector<std::string> index_group_b = split_string(values[1], "/");
				std::vector<std::string> index_group_c = split_string(values[2], "/");
				indices.push_back(index_group{
					.position_index = (unsigned int)atoi(index_group_a[0].c_str())-1,
					.texture_coordinate_index = (unsigned int)atoi(index_group_a[1].c_str())-1,
					.normal_index = (unsigned int)atoi(index_group_a[2].c_str())-1,
				});
				indices.push_back(index_group{
					.position_index = (unsigned int)atoi(index_group_b[0].c_str())-1,
					.texture_coordinate_index = (unsigned int)atoi(index_group_b[1].c_str())-1,
					.normal_index = (unsigned int)atoi(index_group_b[2].c_str())-1,
				});
				indices.push_back(index_group{
					.position_index = (unsigned int)atoi(index_group_c[0].c_str())-1,
					.texture_coordinate_index = (unsigned int)atoi(index_group_c[1].c_str())-1,
					.normal_index = (unsigned int)atoi(index_group_c[2].c_str())-1,
				});
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
	
	vertex* vertex_data = new vertex[indices.size()];
	GLuint* index_data = new GLuint[indices.size()];
	m_num_indices = indices.size();

	for (unsigned int i = 0; i < indices.size(); i++) {
		auto index = std::next(indices.begin(), i);
		auto vertex_position_iter = std::next(vertex_positions.begin(), index->position_index);
		auto vertex_normal_iter = std::next(vertex_normals.begin(), index->normal_index);
		auto vertex_texture_coordinate_iter = std::next(vertex_texture_coordinates.begin(), index->texture_coordinate_index);
		index_data[i] = i;
		vertex_data[i] = vertex{*vertex_position_iter, *vertex_normal_iter, *vertex_texture_coordinate_iter};
	}

	for (unsigned int i = 0; i < indices.size()/3; i++) {
		printf("triangle #%i\n", i+1);
		for (unsigned int j = 0; j < 3; j++) {
			unsigned int index = 3*i+j;
			printf("  vertex #%i\n", j+1);
			printf("    position: (x=%f, y=%f, z=%f)\n", vertex_data[index].position.x, vertex_data[index].position.y, vertex_data[index].position.z);
			printf("    normal: (x=%f, y=%f, z=%f)\n", vertex_data[index].normal.x, vertex_data[index].normal.y, vertex_data[index].normal.z);
			printf("    texture coordinate: (u=%f, v=%f)\n", vertex_data[index].uv.x, vertex_data[index].uv.y);
		}
	}

	glGenBuffers(1, &m_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, m_num_indices*sizeof(vertex), vertex_data, GL_STATIC_DRAW);

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
