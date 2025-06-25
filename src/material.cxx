#include "glcommon.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "gltexture.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include "parse_wad.hpp"
#include "util.hpp"

size_t get_uniform_data_size(UniformType type) {
	switch (type) {
		case UN_BOOL: return sizeof(bool);
		case UN_INT: return sizeof(int);
		case UN_FLOAT: return sizeof(float);
		case UN_VEC2: return sizeof(glm::vec2);
		case UN_VEC3: return sizeof(glm::vec3);
		case UN_VEC4: return sizeof(glm::vec4);
		case UN_MAT4: return sizeof(glm::mat4);
		case UN_IVEC2: return sizeof(glm::ivec2);
		case UN_IVEC3: return sizeof(glm::ivec3);
		case UN_IVEC4: return sizeof(glm::ivec4);
		default: return 1;
	}
}

Material::Material(Game* game, std::string wad_path, std::vector<std::string> data) {
	m_uniforms = std::map<std::string, Uniform>();
	m_shader = nullptr;
	m_texture = nullptr;
	for (std::string line : data) {
		std::string identifier = line.substr(0,line.find(" "));
		std::string path = line.substr(line.find(" ")+1,line.size());
		if (identifier == "shader") {
			m_shader = game->GetResourceManager()->GetShader(path);
		} else if (identifier == "texture") {
			m_texture = game->GetResourceManager()->GetGLTexture(path);
		} else if (identifier == "uniform") {
			std::string name = path.substr(0, path.find(" "));
			std::string info = path.substr(path.find(" ")+1, path.size());
			std::string type_s = info.substr(0, info.find(" "));
			std::string data_s = (!type_s.starts_with("iv") && !type_s.starts_with("v")) ?
				trim_nonstandard(info.substr(info.find(" ")+1,info.size())) :
				info.substr(info.find(" ")+1,info.size());

			if (type_s == "bool") {
				bool data = data_s == "true";
				SetUniform(Uniform{
					name,
					UN_BOOL,
					(void*)&data
				});
			} else if (type_s == "int") {
				int data = std::stoi(data_s);
				SetUniform(Uniform{
					name,
					UN_INT,
					(void*)&data
				});
			} else if (type_s == "float") {
				float data = std::stof(data_s);
				SetUniform(Uniform{
					name,
					UN_FLOAT,
					(void*)&data
				});
			} else if (type_s == "vec2") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::vec2 data{
					stof(components[0]),
					stof(components[1])
				};
				SetUniform(Uniform{
					name,
					UN_VEC2,
					(void*)&data
				});
			} else if (type_s == "vec3") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::vec3 data{
					std::stof(components[0]),
					std::stof(components[1]),
					std::stof(components[2])
				};
				SetUniform(Uniform{
					name,
					UN_VEC3,
					(void*)&data
				});
			} else if (type_s == "vec4") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::vec4 data{
					std::stof(components[0]),
					std::stof(components[1]),
					std::stof(components[2]),
					std::stof(components[3])
				};
				SetUniform(Uniform{
					name,
					UN_VEC4,
					(void*)&data
				});
			} else if (type_s == "ivec2") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::ivec2 data{
					stoi(components[0]),
					stoi(components[1])
				};
				SetUniform(Uniform{
					name,
					UN_IVEC2,
					(void*)&data
				});
			} else if (type_s == "ivec3") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::ivec3 data{
					std::stoi(components[0]),
					std::stoi(components[1]),
					std::stoi(components[2])
				};
				SetUniform(Uniform{
					name,
					UN_IVEC3,
					(void*)&data
				});
			} else if (type_s == "ivec4") {
				std::vector<std::string> components = split_string(data_s, ",");
				for (auto& i : components) i = trim_nonstandard(i);
				glm::ivec4 data{
					std::stoi(components[0]),
					std::stoi(components[1]),
					std::stoi(components[2]),
					std::stoi(components[3])
				};
				SetUniform(Uniform{
					name,
					UN_IVEC4,
					(void*)&data
				});
			}
		}
	}
}

Material::Material(std::shared_ptr<Shader> shader, std::shared_ptr<GLTexture> texture) {
	m_uniforms = std::map<std::string, Uniform>();
	m_shader = shader;
	m_texture = texture;
}

void Material::Cleanup() {}

void Material::SetTexture(std::shared_ptr<GLTexture> texture) {m_texture = texture;}
void Material::SetShader(std::shared_ptr<Shader> shader) {m_shader = shader;}
void Material::SetUniform(Uniform uniform) {
	if (m_uniforms.contains(uniform.name)) {
		free(m_uniforms[uniform.name].data);
		m_uniforms.erase(uniform.name);
	}
	size_t size = get_uniform_data_size(uniform.type);
	void* data = malloc(size);
	memcpy(data, uniform.data, size);
	Uniform n_uniform{uniform.name, uniform.type, data};
	m_uniforms[uniform.name] = n_uniform;
}

void Material::Bind(Game* game) {
	if (m_texture != nullptr) glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
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
		case UN_BOOL: {
			bool data = *(bool *)uniform.data;
			m_shader->UniformBool(uniform.name, data);
			break;
		}
		case UN_INT: {
			int data = *(int *)uniform.data;
			m_shader->UniformInt(uniform.name, data);
			break;
		}
		case UN_FLOAT: {
			float data = *(float *)uniform.data;
			m_shader->UniformFloat(uniform.name, data);
			break;
		}
		case UN_VEC2: {
			glm::vec2 data = *(glm::vec2 *)uniform.data;
			m_shader->UniformVec2(uniform.name, data);
			break;
		}
		case UN_VEC3: {
			glm::vec3 data = *(glm::vec3 *)uniform.data;
			m_shader->UniformVec3(uniform.name, data);
			break;
		}
		case UN_VEC4: {
			glm::vec4 data = *(glm::vec4 *)uniform.data;
			m_shader->UniformVec4(uniform.name, data);
			break;
		}
		case UN_MAT4: {
			glm::mat4 data = *(glm::mat4 *)uniform.data;
			m_shader->UniformMat4(uniform.name, data);
			break;
		}
		case UN_IVEC2: {
			glm::ivec2 data = *(glm::ivec2 *)uniform.data;
			m_shader->UniformIVec2(uniform.name, data);
			break;
		}
		case UN_IVEC3: {
			glm::ivec3 data = *(glm::ivec3 *)uniform.data;
			m_shader->UniformIVec3(uniform.name, data);
			break;
		}
		case UN_IVEC4: {
			glm::ivec4 data = *(glm::ivec4 *)uniform.data;
			m_shader->UniformIVec4(uniform.name, data);
			break;
		}
		default: {
			break;
		}
	}
}

std::shared_ptr<GLTexture> Material::GetTexture() {return m_texture;}
std::shared_ptr<Shader> Material::GetShader() {return m_shader;}
