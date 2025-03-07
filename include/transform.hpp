#pragma once
#include <glm/ext.hpp>

class Transform {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 GetModelMatrix() {
		glm::mat4 model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, position);
		model_matrix *= glm::mat4_cast(glm::quat(rotation));
		model_matrix = glm::scale(model_matrix, scale);
		return model_matrix;
	}
};
