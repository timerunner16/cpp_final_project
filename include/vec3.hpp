#pragma once
#include <glm/ext.hpp>
#include <string>

class ivec3;

struct vec3 : public glm::vec3 {
	using glm::vec3::vec;

	vec3(const glm::vec3& obj);
	vec3(const ivec3& obj);

	void set_x(float x);
	void set_y(float y);
	void set_z(float z);

	float get_x();
	float get_y();
	float get_z();

	vec3 add(const vec3& other);
	vec3 sub(const vec3& other);
	vec3 mul(const vec3& other);
	vec3 div(const vec3& other);

	vec3 mulf(const float& other);
	vec3 divf(const float& other);

	float length() const;
	float length_squared() const;
	float dot(const vec3& other) const;
	vec3 unit() const;
	vec3 cross(const vec3& other) const;

	std::string to_string() {
		return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
	}
};
