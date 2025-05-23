#include "physics.hpp"
#include <cstdio>

#define EPSILON 0.005f
#define SIDE_EPSILON 0.05f

vec2 closest_point_on_line(vec2 p, line l) {
	vec2 delta = (l.b - l.a);
	vec2 delta_n = delta.unit();
	vec2 lhs = p - l.a;
	float dot = lhs.dot(delta_n);
	return l.a + delta_n * dot;
}

collision_result sweep_box_line(Box a, line l, vec2 v) {
	vec2 linedir = l.b - l.a;
	vec2 linemin = vec2(0,0);
	vec2 linemax = vec2(0,0);

	if (linedir.x > 0) {
		linemin.x = l.a.x;
		linemax.x = l.b.x;
	} else {
		linemin.x = l.b.x;
		linemax.x = l.a.x;
	}
	if (linedir.y > 0) {
		linemin.y = l.a.y;
		linemax.y = l.b.y;
	} else {
		linemin.y = l.b.y;
		linemax.y = l.a.y;
	}
	
	vec2 ln = l.n();

	vec2 out_velocity = vec2();
	vec2 hit_normal = ln;

	float hit_t = 0.0f;
	float out_t = 1.0f;

	vec2 min = a.min();
	vec2 max = a.max();

	float r = a.bound.x * fabs(ln.x) + a.bound.y * fabs(ln.y);
	vec2 d = l.a - a.center;
	float box_p = d.dot(ln);
	float v_p = v.dot(ln);

	r *= glm::sign(v_p);

	hit_t = glm::max((box_p-r)/v_p, hit_t);
	out_t = glm::min((box_p+r)/v_p, out_t);
	
	if (v.x < 0) {
		if (max.x < linemin.x) return collision_result{false};
		hit_t = glm::max((linemax.x - min.x) / v.x, hit_t);
		out_t = glm::min((linemin.x - max.x) / v.x, out_t);
	} else if (v.x > 0) {
		if (min.x > linemax.x) return collision_result{false};
		hit_t = glm::max((linemin.x - max.x) / v.x, hit_t);
		out_t = glm::min((linemax.x - min.x) / v.x, out_t);
	} else {
		if (linemin.x > max.x || linemax.x < min.x) return collision_result{false};
	}

	if (hit_t > out_t) return collision_result{false};

	if (v.y < 0) {
		if (max.y < linemin.y) return collision_result{false};
		hit_t = glm::max((linemax.y - min.y) / v.y, hit_t);
		out_t = glm::min((linemin.y - max.y) / v.y, out_t);
	} else if (v.y > 0) {
		if (min.y > linemax.y) return collision_result{false};
		hit_t = glm::max((linemin.y - max.y) / v.y, hit_t);
		out_t = glm::min((linemax.y - min.y) / v.y, out_t);
	} else {
		if (linemin.y > max.y || linemax.y < min.y) return collision_result{false};
	}

	if (hit_t > out_t) return collision_result{false};

	vec2 until_blocked = v * glm::max(hit_t, 0.0f);
	
	vec2 closest_corner = a.bound;
	if (ln.x > 0) closest_corner.x *= -1;
	if (ln.y > 0) closest_corner.y *= -1;

	vec2 resolved_corner = a.center + v + closest_corner;
	vec2 projected_corner = closest_point_on_line(resolved_corner, l);
	vec2 projected_center = projected_corner - closest_corner;
	vec2 difference_center = projected_center - a.center;

	out_velocity = difference_center - until_blocked;

	if (fabs(until_blocked.x + a.center.x - a.bound.x - linemax.x) < SIDE_EPSILON) {
		out_velocity.x = v.x > 0 ? v.x : 0;
		out_velocity.y = v.y;
	} else if (fabs(until_blocked.x + a.center.x + a.bound.x - linemin.x) < SIDE_EPSILON) {
		out_velocity.x = v.x < 0 ? v.x : 0;
		out_velocity.y = v.y;
	}

	if (fabs(until_blocked.y + a.center.y - a.bound.y - linemax.y) < SIDE_EPSILON) {
		out_velocity.y = v.y > 0 ? v.y : 0;
		out_velocity.x = v.x;
	} else if (fabs(until_blocked.y + a.center.y + a.bound.y - linemin.y) < SIDE_EPSILON) {
		out_velocity.y = v.y < 0 ? v.y : 0;
		out_velocity.x = v.x;
	}

	return collision_result{true, until_blocked, out_velocity, hit_normal};
}

collision_result discrete_box_box(Box moving, Box unmoving, vec2 v) {
	vec2 mmin = moving.min() + v;
	vec2 umin = unmoving.min();
	vec2 mmax = moving.max() + v;
	vec2 umax = unmoving.max();

	if (mmax.x < umin.x || mmin.x > umax.x || mmax.y < umin.y || mmin.y > umax.y)
		return collision_result{false};

	vec2 r_left{umin.x-mmax.x, 0};
	vec2 r_right{umax.x-mmin.x, 0};
	vec2 r_down{0, umin.y-mmax.y};
	vec2 r_up{0, umax.y-mmin.y};

	float l_r_left = r_left.length();
	float l_r_right = r_right.length();
	float l_r_down = r_down.length();
	float l_r_up = r_up.length();

	if (l_r_left < l_r_right && l_r_left < l_r_down && l_r_left < l_r_up)
		return collision_result{true, r_left, vec2{0,0}, vec2{1,0}};
	else if (l_r_right < l_r_left && l_r_right < l_r_down && l_r_right < l_r_up)
		return collision_result{true, r_right, vec2{0,0}, vec2{-1,0}};
	else if (l_r_down < l_r_left && l_r_down < l_r_right && l_r_down < l_r_up)
		return collision_result{true, r_down, vec2{0,0}, vec2{0,1}};
	else
		return collision_result{true, r_up, vec2{0,0}, vec2{0,-1}};
	return collision_result{true, vec2{0,0}, vec2{0,0}, vec2{0,0}};
}
