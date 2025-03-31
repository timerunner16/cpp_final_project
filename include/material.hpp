#pragma once
#include <memory>
#include <string>
#include "resource_manager.hpp"

class Game;
class GLTexture;
class Shader;

enum UniformType {
	BOOL,
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	MAT4,
	IVEC2,
	IVEC3,
	IVEC4,
};

struct Uniform {
	std::string name;
	UniformType type;
	void* data;
};

class Material : public GenericResource {
public:
	Material(Game* game, std::string file_path);

	void Cleanup();

	void SetTexture(std::shared_ptr<GLTexture> texture);
	void SetShader(std::shared_ptr<Shader> shader);
	void SetUniform(Uniform uniform);

	void Bind(Game* game);
private:
	void ApplyUniform(const Uniform& uniform);

	std::map<std::string, Uniform> m_uniforms;
	std::shared_ptr<GLTexture> m_texture;
	std::shared_ptr<Shader> m_shader;
};
