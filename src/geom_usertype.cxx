#include "geom_usertype.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "transform.hpp"
#include "physics.hpp"
#include "game_object.hpp"
#include "map.hpp"

void geom_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject* game_object) {
	sol::usertype<vec2> vec2_data_type = lua_state->new_usertype<vec2>(
		"Vector2",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec2>();
			},
			[](const float& value) {
				return std::make_shared<vec2>(value);
			},
			[](const float& x, const float& y) {
				return std::make_shared<vec2>(x, y);
			},
			[](const ivec2& obj) {
				return std::make_shared<vec2>(obj);
			}
		),
		"x", sol::property(&vec2::get_x, &vec2::set_x),
		"y", sol::property(&vec2::get_y, &vec2::set_y),
		sol::meta_function::addition, &vec2::add,
		sol::meta_function::subtraction, &vec2::sub,
		sol::meta_function::multiplication, sol::overload(&vec2::mul, &vec2::mulf),
		sol::meta_function::division, sol::overload(&vec2::div, &vec2::divf),
		sol::meta_function::unary_minus, &vec2::unary_minus,
		"plus", &vec2::add,
		"minus", &vec2::sub,
		"times", sol::overload(&vec2::mul, &vec2::mulf),
		"div", sol::overload(&vec2::div, &vec2::divf),
		"length", sol::readonly_property(&vec2::length),
		"unit", sol::readonly_property(&vec2::unit),
		"dot", &vec2::dot,
		"withX", &vec2::withX,
		"withY", &vec2::withY
	);

	sol::usertype<vec3> vec3_data_type = lua_state->new_usertype<vec3>(
		"Vector3",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec3>();
			},
			[](const float& value) {
				return std::make_shared<vec3>(value);
			},
			[](const float& x, const float& y, const float& z) {
				return std::make_shared<vec3>(x, y, z);
			},
			[](const ivec3& obj) {
				return std::make_shared<vec3>(obj);
			}
		),
		"x", sol::property(&vec3::get_x, &vec3::set_x),
		"y", sol::property(&vec3::get_y, &vec3::set_y),
		"z", sol::property(&vec3::get_z, &vec3::set_z),
		sol::meta_function::addition, &vec3::add,
		sol::meta_function::subtraction, &vec3::sub,
		sol::meta_function::multiplication, sol::overload(&vec3::mul, &vec3::mulf),
		sol::meta_function::division, sol::overload(&vec3::div, &vec3::divf),
		sol::meta_function::unary_minus, &vec3::unary_minus,
		"plus", &vec3::add,
		"minus", &vec3::sub,
		"times", sol::overload(&vec3::mul, &vec3::mulf),
		"div", sol::overload(&vec3::div, &vec3::divf),
		"length", sol::readonly_property(&vec3::length),
		"unit", sol::readonly_property(&vec3::unit),
		"dot", &vec3::dot,
		"withX", &vec3::withX,
		"withY", &vec3::withY,
		"withZ", &vec3::withZ
	);
	sol::usertype<vec4> vec4_data_type = lua_state->new_usertype<vec4>(
		"Vector4",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec4>();
			},
			[](const float& value) {
				return std::make_shared<vec4>(value);
			},
			[](const float& x, const float& y, const float& z, const float& w) {
				return std::make_shared<vec4>(x, y, z, w);
			}
		),
		"x", sol::property(&vec4::get_x, &vec4::set_x),
		"y", sol::property(&vec4::get_y, &vec4::set_y),
		"z", sol::property(&vec4::get_z, &vec4::set_z),
		"w", sol::property(&vec4::get_w, &vec4::set_w)
	);

	sol::usertype<ivec2> ivec2_data_type = lua_state->new_usertype<ivec2>(
		"IVector2",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec2>();
			},
			[](const int& value) {
				return std::make_shared<vec2>(value);
			},
			[](const int& x, const int& y) {
				return std::make_shared<vec2>(x, y);
			},
			[](const vec2& obj) {
				return std::make_shared<ivec2>(obj);
			}
		),
		"x", sol::property(&ivec2::get_x, &ivec2::set_x),
		"y", sol::property(&ivec2::get_y, &ivec2::set_y),
		sol::meta_function::addition, &ivec2::add,
		sol::meta_function::subtraction, &ivec2::sub,
		sol::meta_function::multiplication, sol::overload(&ivec2::mul, &ivec2::mulf),
		sol::meta_function::division, sol::overload(&ivec2::div, &ivec2::divf),
		sol::meta_function::unary_minus, &ivec2::unary_minus,
		"plus", &ivec2::add,
		"minus", &ivec2::sub,
		"times", sol::overload(&ivec2::mul, &ivec2::mulf),
		"div", sol::overload(&ivec2::div, &ivec2::divf),
		"length", &ivec2::length,
		"withX", &ivec2::withX,
		"withY", &ivec2::withY
	);

	sol::usertype<ivec3> ivec3_data_type = lua_state->new_usertype<ivec3>(
		"IVector3",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<vec3>();
			},
			[](const int& value) {
				return std::make_shared<vec3>(value);
			},
			[](const int& x, const int& y, const int& z) {
				return std::make_shared<vec3>(x, y, z);
			},
			[](const vec3& obj) {
				return std::make_shared<ivec3>(obj);
			}
		),
		"x", sol::property(&ivec3::get_x, &ivec3::set_x),
		"y", sol::property(&ivec3::get_y, &ivec3::set_y),
		"z", sol::property(&ivec3::get_z, &ivec3::set_z),
		sol::meta_function::addition, &ivec3::add,
		sol::meta_function::subtraction, &ivec3::sub,
		sol::meta_function::multiplication, &ivec3::mul,
		sol::meta_function::division, &ivec3::div,
		sol::meta_function::unary_minus, &ivec3::unary_minus,
		"plus", &ivec3::add,
		"minus", &ivec3::sub,
		"times", sol::overload(&ivec3::mul, &ivec3::mulf),
		"div", sol::overload(&ivec3::div, &ivec3::divf),
		"length", &ivec3::length,
		"withX", &ivec3::withX,
		"withY", &ivec3::withY,
		"withZ", &ivec3::withZ
	);

	sol::usertype<Transform> transform_data_type = lua_state->new_usertype<Transform>(
		"Transform",
		sol::meta_function::construct,
		sol::factories(
			[]() {
				return std::make_shared<Transform>();
			},
			[](const vec3& position, const vec3& rotation, const vec3& scale) {
				return std::make_shared<Transform>(position, rotation, scale);
			}
		),
		"Position", &Transform::position,
		"Rotation", &Transform::rotation,
		"Scale", &Transform::scale,

		"LookVector", sol::readonly_property(&Transform::GetLookVector),
		"RightVector", sol::readonly_property(&Transform::GetRightVector),
		"UpVector", sol::readonly_property(&Transform::GetUpVector)
	);

	sol::usertype<collision_result> collision_result_data_type = lua_state->new_usertype<collision_result>(
		"CollisionResult", sol::no_constructor,
		"Hit", sol::readonly_property(&collision_result::hit),
		"Position", sol::readonly_property(&collision_result::until_blocked),
		"Normal", sol::readonly_property(&collision_result::hit_normal),
		"Instance", sol::readonly_property(&collision_result::game_object)
	);

	sol::usertype<Sector> sector_data_type = lua_state->new_usertype<Sector>(
		"Sector", sol::no_constructor,
		"HeightFloor", sol::readonly_property(&Sector::heightfloor),
		"HeightCeiling", sol::readonly_property(&Sector::heightfloor)
	);
}
