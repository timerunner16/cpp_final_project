#include <GL/glew.h>
#include <GL/glu.h>
#include <fstream>
#include "material.hpp"
#include "shader.hpp"
#include "gltexture.hpp"
#include "game.hpp"

Material::Material(Game* game, std::string file_path) {
	std::ifstream infile(file_path);
	m_uniforms = std::map<std::string, Uniform>();
	m_shader = nullptr;
	m_texture = nullptr;
	std::string line;
	while (std::getline(infile, line)) {
		std::string identifier = line.substr(0,line.find(" "));
		std::string path = line.substr(line.find(" ")+1,line.size());
		if (identifier == "shader") {
			m_shader = game->GetResourceManager()->GetResource<Shader>(path);
		} else if (identifier == "texture") {
			m_texture = game->GetResourceManager()->GetResource<GLTexture>(path);
		}
	}
}

void Material::Cleanup() {}

void Material::SetTexture(std::shared_ptr<GLTexture> texture) {m_texture = texture;}
void Material::SetShader(std::shared_ptr<Shader> shader) {m_shader = shader;}
void Material::SetUniform(Uniform uniform) {m_uniforms[uniform.name] = uniform;}

void Material::Bind(Game* game) {
	glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
	glUseProgram(m_shader->GetProgramID());

	for (auto i = m_uniforms.begin(); i != m_uniforms.end(); i++) {
		Uniform uniform = i->second;
		ApplyUniform(uniform);
	}
	std::map<std::string, Uniform>* global_uniforms = game->GetGlobalUniforms();
	for (auto i = global_uniforms->begin(); i != global_uniforms->end(); i++) {
		Uniform uniform = i->second;
		ApplyUniform(uniform);
	}
}

void Material::ApplyUniform(const Uniform& uniform) {
	switch (uniform.type) {
		case BOOL: {
			bool data = *(bool *)uniform.data;
			m_shader->UniformBool(uniform.name, data);
			break;
		}
		case INT: {
			int data = *(int *)uniform.data;
			m_shader->UniformInt(uniform.name, data);
			break;
		}
		case FLOAT: {
			float data = *(float *)uniform.data;
			m_shader->UniformFloat(uniform.name, data);
			break;
		}
		case VEC2: {
			glm::vec2 data = *(glm::vec2 *)uniform.data;
			m_shader->UniformVec2(uniform.name, data);
			break;
		}
		case VEC3: {
			glm::vec3 data = *(glm::vec3 *)uniform.data;
			m_shader->UniformVec3(uniform.name, data);
			break;
		}
		case VEC4: {
			glm::vec4 data = *(glm::vec4 *)uniform.data;
			m_shader->UniformVec4(uniform.name, data);
			break;
		}
		case MAT4: {
			glm::mat4 data = *(glm::mat4 *)uniform.data;
			m_shader->UniformMat4(uniform.name, data);
			break;
		}
		case IVEC2: {
			glm::ivec2 data = *(glm::ivec2 *)uniform.data;
			m_shader->UniformIVec2(uniform.name, data);
			break;
		}
		case IVEC3: {
			glm::ivec3 data = *(glm::ivec3 *)uniform.data;
			m_shader->UniformIVec3(uniform.name, data);
			break;
		}
		case IVEC4: {
			glm::ivec4 data = *(glm::ivec4 *)uniform.data;
			m_shader->UniformIVec4(uniform.name, data);
			break;
		}
		default: {
			break;
		}
	}
}

