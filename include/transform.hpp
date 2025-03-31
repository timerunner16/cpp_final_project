#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext.hpp>
#include "vec3.hpp"

class Transform {
public:
	Transform();
	Transform(vec3 position, vec3 rotation, vec3 scale);

	vec3 position;
	vec3 rotation;
	vec3 scale;

	void set_position(vec3 position);
	void set_rotation(vec3 rotation);
	void set_scale(vec3 scale);

	vec3& get_position();
	vec3& get_rotation();
	vec3& get_scale();

	glm::mat4 GetModelMatrix();
	vec3 GetLookVector();
};
