#pragma once
#include <glm/ext.hpp>

class vec3;

struct ivec3 : public glm::ivec3 {
	using glm::ivec3::vec;

	ivec3(const vec3& obj);

	void set_x(int x);
	void set_y(int y);
	void set_z(int z);

	int get_x();
	int get_y();
	int get_z();

	ivec3 add(const ivec3& other);
	ivec3 sub(const ivec3& other);
	ivec3 mul(const ivec3& other);
	ivec3 div(const ivec3& other);
};
