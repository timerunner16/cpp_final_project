#pragma once
#include <glm/ext.hpp>
#include <string>

struct ivec2;

struct vec2 : public glm::vec2 {
	using glm::vec2::vec;

	vec2(const glm::vec2& obj);
	vec2(const ivec2& obj);

	void set_x(float x);
	void set_y(float y);

	float get_x();
	float get_y();

	vec2 add(const vec2& other);
	vec2 sub(const vec2& other);
	vec2 mul(const vec2& other);
	vec2 div(const vec2& other);

	vec2 mulf(const float& other);
	vec2 divf(const float& other);

	vec2 unary_minus();

	float length() const;
	float length_squared() const;
	float dot(const vec2& other) const;
	vec2 unit() const;

	vec2 withX(float x);
	vec2 withY(float y);

	std::string to_string() {
		return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
	}
};
