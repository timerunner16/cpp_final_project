#pragma once
#include <variant>
#include <map>
#include <sol/sol.hpp>

struct vec2;
struct vec3;
struct ivec2;
struct ivec3;
struct particle_system_create_info;
class Transform;
struct Box;
class Mesh;
class GLTexture;
class Shader;
class Material;
struct Uniform;

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
