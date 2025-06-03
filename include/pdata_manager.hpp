#pragma once
#include <variant>
#include <map>
#include <sol/sol.hpp>

class vec2;
class vec3;
class ivec2;
class ivec3;
class particle_system_create_info;
class Transform;
class Box;
class Mesh;
class GLTexture;
class Shader;
class Material;
class Uniform;

typedef std::variant<
	int, bool, float,
	vec2, vec3, ivec2, ivec3,
	particle_system_create_info,
	Transform, Box,
	Mesh, GLTexture, Shader, Material, Uniform,
	sol::nil_t
> pdata_value;

class PDataManager {
public:
	PDataManager();
	~PDataManager();

	void SetPData(std::string key, pdata_value value);
	pdata_value GetPData(std::string key);
private:
	std::map<std::string, pdata_value> m_pdata_values;
};
