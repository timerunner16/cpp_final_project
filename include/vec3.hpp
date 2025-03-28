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
};
