#pragma once
#include <glm/ext.hpp>

class ivec2;

struct vec2 : public glm::vec2 {
	using glm::vec2::vec;

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
};
