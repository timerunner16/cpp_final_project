#include "transform.hpp"

Transform::Transform() {
	position = vec3(0.0f);
	rotation = vec3(0.0f);
	scale = vec3(1.0f);
}

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::mat4 Transform::GetModelMatrix() const {
	glm::mat4 translation_mat = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotation_mat = glm::eulerAngleZYX(rotation.z, rotation.y, rotation.x);
	glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
	return translation_mat * rotation_mat * scale_mat;
}

vec3 Transform::GetLookVector() const {
	return glm::normalize(vec3{
	   -glm::cos(rotation.x) * glm::sin(rotation.y),
		glm::sin(rotation.x),
	    glm::cos(rotation.x) * glm::cos(rotation.y),
	});
}

vec3 Transform::GetRightVector() const {
	vec3 lookvec = GetLookVector();
	return -glm::normalize(glm::cross(lookvec,glm::vec3(0.0f,1.0f,0.0f)));
}

vec3 Transform::GetUpVector() const {
	vec3 lookvec = GetLookVector();
	vec3 rightvec = glm::normalize(glm::cross(lookvec,glm::vec3(0.0f,1.0f,0.0f)));
	return glm::normalize(glm::cross(rightvec, lookvec));
}

void Transform::set_position(vec3 position) {this->position = position;}
void Transform::set_rotation(vec3 rotation) {this->rotation = rotation;}
void Transform::set_scale(vec3 scale) {this->scale = scale;}

vec3& Transform::get_position() {return position;}
vec3& Transform::get_rotation() {return rotation;}
vec3& Transform::get_scale() {return scale;}
