#include "vdata_usertype.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "gltexture.hpp"
#include "material.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"

void vdata_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject *game_object) {
	sol::usertype<Mesh> mesh_data_type = lua_state->new_usertype<Mesh>("Mesh", sol::no_constructor);
	sol::usertype<GLTexture> texture_data_type = lua_state->new_usertype<GLTexture>("Texture", sol::no_constructor);
	sol::usertype<Shader> shader_data_type = lua_state->new_usertype<Shader>("Shader", sol::no_constructor);
	sol::usertype<Uniform> uniform_data_type = lua_state->new_usertype<Uniform>(
		"Uniform",
		sol::factories(
			[](const std::string& name, const bool& data) {
				return std::make_shared<Uniform>(name, UN_BOOL, (void*)(&data));
			},
			[](const std::string& name, const int& data) {
				return std::make_shared<Uniform>(name, UN_INT, (void*)(&data));
			},
			[](const std::string& name, const float& data) {
				return std::make_shared<Uniform>(name, UN_FLOAT, (void*)(&data));
			},
			[](const std::string& name, const vec2& data) {
				return std::make_shared<Uniform>(name, UN_VEC2, (void*)(&data));
			},
			[](const std::string& name, const vec3& data) {
				return std::make_shared<Uniform>(name, UN_VEC3, (void*)(&data));
			},
			[](const std::string& name, const vec4& data) {
				return std::make_shared<Uniform>(name, UN_VEC4, (void*)(&data));
			},
			[](const std::string& name, const ivec2& data) {
				return std::make_shared<Uniform>(name, UN_IVEC2, (void*)(&data));
			},
			[](const std::string& name, const ivec3& data) {
				return std::make_shared<Uniform>(name, UN_IVEC3, (void*)(&data));
			}
		)
	);

	sol::usertype<Material> material_data_type = lua_state->new_usertype<Material>("Material",
		sol::no_constructor,
		"SetUniform", &Material::SetUniform,
		"SetTexture", &Material::SetTexture,
		"SetShader", &Material::SetShader
	);
}
