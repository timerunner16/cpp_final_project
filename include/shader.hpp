#pragma once
#include "glcommon.hpp"
#include <glm/ext.hpp>
#include <string>
#include "resource_manager.hpp"

class Shader : public GenericResource {
public:
	Shader(uint8_t* data, uint32_t size);
	
	void Cleanup();

	void UniformBool(std::string name, bool value);
	void UniformInt(std::string name, int value);
	void UniformFloat(std::string name, float value);
	void UniformVec2(std::string name, glm::vec2 value);
	void UniformVec3(std::string name, glm::vec3 value);
	void UniformVec4(std::string name, glm::vec4 value);
	void UniformMat4(std::string name, glm::mat4 value);
	void UniformIVec2(std::string name, glm::ivec2 value);
	void UniformIVec3(std::string name, glm::ivec3 value);
	void UniformIVec4(std::string name, glm::ivec4 value);

	GLuint GetProgramID();
private:
	GLuint m_program_id;
};
