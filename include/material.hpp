#pragma once
#include <memory>
#include <string>
#include <vector>
#include "resource_manager.hpp"

class Game;
class GLTexture;
class Shader;

enum UniformType {
	UN_BOOL,
	UN_INT,
	UN_FLOAT,
	UN_VEC2,
	UN_VEC3,
	UN_VEC4,
	UN_MAT4,
	UN_IVEC2,
	UN_IVEC3,
	UN_IVEC4,
};

struct Uniform {
	std::string name="";
	UniformType type=UN_INT;
	void* data=nullptr;
};

size_t get_uniform_data_size(UniformType type);

class Material : public GenericResource {
public:
	Material(Game* game, std::string wad_path, std::vector<std::string> data);
	Material(std::shared_ptr<Shader>, std::shared_ptr<GLTexture>);

	void Cleanup();

	void SetTexture(std::shared_ptr<GLTexture> texture);
	void SetShader(std::shared_ptr<Shader> shader);
	void SetUniform(Uniform uniform);

	std::shared_ptr<GLTexture> GetTexture();
	std::shared_ptr<Shader> GetShader();

	void Bind(Game* game);
	void ApplyUniform(const Uniform& uniform);
private:
	std::map<std::string, Uniform> m_uniforms;
	std::shared_ptr<GLTexture> m_texture;
	std::shared_ptr<Shader> m_shader;
};
