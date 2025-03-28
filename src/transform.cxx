#include "transform.hpp"

glm::mat4 Transform::GetModelMatrix() {
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::scale(model_matrix, scale);
	model_matrix *= glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
	return model_matrix;
}

vec3 Transform::GetLookVector() {
	glm::vec3 normalized = glm::normalize(glm::vec3{
		glm::cos(rotation.x) * glm::cos(rotation.y),
		glm::sin(rotation.x),
	   -glm::cos(rotation.x) * glm::sin(rotation.y),
	});
	return vec3{normalized.x, normalized.y, normalized.z};
}

void Transform::set_position(vec3 position) {this->position = position;}
void Transform::set_rotation(vec3 rotation) {this->rotation = rotation;}
void Transform::set_scale(vec3 scale) {this->scale = scale;}

vec3& Transform::get_position() {return position;}
vec3& Transform::get_rotation() {return rotation;}
vec3& Transform::get_scale() {return scale;}
