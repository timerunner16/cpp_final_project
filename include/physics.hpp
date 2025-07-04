#pragma once
#include "vec2.hpp"

#define EPSILON 0.005f
#define SIDE_EPSILON 0.05f

class GameObject;

struct Box {
	vec2 bound;
	vec2 center;
	vec2 max() {return center + bound;}
	vec2 min() {return center - bound;}
};

struct line {
	vec2 a;
	vec2 b;
	float floor;
	float midfloor;
	float midceiling;
	float ceiling;
	bool lower;
	bool mid;
	bool higher;
	vec2 n() {
		vec2 delta = a-b;
		vec2 unit = delta.unit();
		return vec2{-unit.y,unit.x};
	}
};

struct triangle {
	vec2 a, b, c;
};

struct collision_result {
	bool hit;
	vec2 until_blocked;
	vec2 out_velocity;
	vec2 hit_normal;
	GameObject* game_object;
};

vec2 closest_point_on_line(vec2 p, line l);

collision_result sweep_box_line(Box a, line l, vec2 v);
collision_result discrete_box_box(Box moving, Box unmoving, vec2 v);
collision_result discrete_line_box(Box a, line ray);
collision_result discrete_line_line(line ray, line segment);

bool overlap_box_triangle(Box a, triangle b);
