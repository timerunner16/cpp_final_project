#pragma once
#include <glm/ext.hpp>
#include <string>

class vec3;

struct ivec3 : public glm::ivec3 {
	using glm::ivec3::vec;

	ivec3(const glm::ivec3& obj);
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

	ivec3 mulf(const float& other);
	ivec3 divf(const float& other);

	ivec3 unary_minus();

	float length() const;
	float length_squared() const;

	std::string to_string() {
		return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
	}
};
