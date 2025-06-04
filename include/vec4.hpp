#pragma once
#include <glm/ext.hpp>
#include <string>

struct vec4 : public glm::vec4 {
	using glm::vec4::vec;

	vec4(const glm::vec4& obj);

	void set_x(float x);
	void set_y(float y);
	void set_z(float z);
	void set_w(float w);

	float get_x();
	float get_y();
	float get_z();
	float get_w();
	
	std::string to_string() {
		return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + "}";
	}
};
