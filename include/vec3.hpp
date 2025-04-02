#pragma once
#include <glm/ext.hpp>

struct vec3 : public glm::vec3 {
	using glm::vec3::vec;

	void set_x(float x) {this->x = x;}
	void set_y(float y) {this->y = y;}
	void set_z(float z) {this->z = z;}

	float get_x() {return x;}
	float get_y() {return y;}
	float get_z() {return z;}

	vec3 add(const vec3& other) {return vec3(x+other.x, y+other.y, z+other.z);}
	vec3 sub(const vec3& other) {return vec3(x-other.x, y-other.y, z-other.z);}
	vec3 mul(const vec3& other) {return vec3(x*other.x, y*other.y, z*other.z);}
	vec3 div(const vec3& other) {return vec3(x/other.x, y/other.y, z/other.z);}

	vec3 mulf(const float& other) {return vec3(x*other, y*other, z*other);}
	vec3 divf(const float& other) {return vec3(x/other, y/other, z/other);}
};
