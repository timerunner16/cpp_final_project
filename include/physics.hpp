#pragma once
#include "vec2.hpp"

struct Box {
	vec2 bound;
	vec2 center;
	vec2 max() {return center + bound;}
	vec2 min() {return center - bound;}
};

struct line {
	vec2 a;
	vec2 b;
	vec2 n() {
		vec2 delta = a-b;
		vec2 unit = delta.unit();
		return vec2{-unit.y,unit.x};
	}
};

struct collision_result {
	bool hit;
	vec2 until_blocked;
	vec2 out_velocity;
	vec2 hit_normal;
};

vec2 closest_point_on_line(vec2 p, line l);

collision_result sweep_box_line(Box a, line l, vec2 v);

void resolve_collisions(std::vector<line> lines, std::vector<Box> boxes);
