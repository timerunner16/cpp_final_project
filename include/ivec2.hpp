#pragma once
#include <glm/ext.hpp>
#include <string>

class vec2;

struct ivec2 : public glm::ivec2 {
	using glm::ivec2::vec;

	ivec2(const glm::ivec2& obj);
	ivec2(const vec2& obj);

	void set_x(int x);
	void set_y(int y);

	int get_x();
	int get_y();

	ivec2 add(const ivec2& other);
	ivec2 sub(const ivec2& other);
	ivec2 mul(const ivec2& other);
	ivec2 div(const ivec2& other);

	ivec2 mulf(const float& other);
	ivec2 divf(const float& other);

	float length() const;

	std::string to_string() {
		return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
	}
};
