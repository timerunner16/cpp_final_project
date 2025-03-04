#include "shader.hpp"
#include <fstream>
#include <sstream>

const GLchar* default_vertex_shader_source = "#version 330 core\nlayout (location=0) in vec3 vertex_position;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main() {\ngl_Position = projection * view * model * vec4(vertex_position, 1.0);\n}";
const GLchar* default_fragment_shader_source = "#version 330 core\nout vec4 fragment;\nvoid main() {\nfragment = vec4(1.0,1.0,1.0,1.0);\n}";

Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path) {
	std::string vertex_shader_source;
	std::string fragment_shader_source;

	std::ifstream vertex_shader_source_file;
	std::ifstream fragment_shader_source_file;

	vertex_shader_source_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_source_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertex_shader_source_file.open(vertex_shader_path);
		fragment_shader_source_file.open(fragment_shader_path);

		std::stringstream vertex_shader_source_stream, fragment_shader_source_stream;

		vertex_shader_source_stream << vertex_shader_source_file.rdbuf();
		fragment_shader_source_stream << fragment_shader_source_file.rdbuf();

		vertex_shader_source_file.close();
		fragment_shader_source_file.close();

		vertex_shader_source = vertex_shader_source_stream.str();
		fragment_shader_source = fragment_shader_source_stream.str();
	} catch (std::ifstream::failure exception) {
		printf("Unable to read shader files!\n");
	}

	const GLchar* vertex_shader_source_char;
	if (!vertex_shader_source.empty()) vertex_shader_source_char = vertex_shader_source.c_str();
	else vertex_shader_source_char = default_vertex_shader_source;

	const GLchar* fragment_shader_source_char;
	if (!fragment_shader_source.empty()) fragment_shader_source_char = fragment_shader_source.c_str();
	else fragment_shader_source_char = default_fragment_shader_source;

	m_program_id = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source_char, NULL);
	glCompileShader(vertex_shader);

	GLint vertex_shader_compiled = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_compiled);
	if (vertex_shader_compiled != GL_TRUE) {
		char info_log[512];
		glGetProgramInfoLog(m_program_id, 512, NULL, info_log);
		printf("Unable to compile vertex shader!\n%s", info_log);
	} else glAttachShader(m_program_id, vertex_shader);

	GLint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source_char, NULL);
	glCompileShader(fragment_shader);

	GLint fragment_shader_compiled = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_compiled);
	if (fragment_shader_compiled != GL_TRUE) {
		char info_log[512];
		glGetProgramInfoLog(m_program_id, 512, NULL, info_log);
		printf("Unable to compile fragment shader!\n%s", info_log);
	} else glAttachShader(m_program_id, fragment_shader);

	glLinkProgram(m_program_id);

	GLint program_linked = GL_FALSE;
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE) printf("Unable to link program!\n");

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader() {
	glDeleteProgram(m_program_id);
}

void Shader::UniformBool(std::string name, bool value) {
	glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), (int)value);
}
void Shader::UniformInt(std::string name, int value) {
	glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), value);
}
void Shader::UniformFloat(std::string name, float value) {
	glUniform1f(glGetUniformLocation(m_program_id, name.c_str()), value);
}
void Shader::UniformVec2(std::string name, glm::vec2 value) {
	glUniform2f(glGetUniformLocation(m_program_id, name.c_str()), value.x, value.y);
}
void Shader::UniformVec3(std::string name, glm::vec3 value) {
	glUniform3f(glGetUniformLocation(m_program_id, name.c_str()), value.x, value.y, value.z);
}
void Shader::UniformMat4(std::string name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::GetProgramID() {
	return m_program_id;
}
