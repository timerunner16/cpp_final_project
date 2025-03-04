#pragma once
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/ext.hpp>
#include <string>

class Shader {
public:
	Shader(std::string vertex_shader_path, std::string fragment_shader_path);
	~Shader();

	void UniformBool(std::string name, bool value);
	void UniformInt(std::string name, int value);
	void UniformFloat(std::string name, float value);
	void UniformVec2(std::string name, glm::vec2 value);
	void UniformVec3(std::string name, glm::vec3 value);
	void UniformVec4(std::string name, glm::vec4 value);
	void UniformMat4(std::string name, glm::mat4 value);

	GLuint GetProgramID();
private:
	GLuint m_program_id;
};
