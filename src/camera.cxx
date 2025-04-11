#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include "camera.hpp"
#include "game.hpp"
#include "window.hpp"

Camera::Camera(Game* game, const Transform& transform, const float& fov) {
	m_game = game;
	m_transform = transform;
	m_fov = fov;
}

Transform& Camera::GetTransform() {
	return m_transform;
}

void Camera::SetTransform(const Transform& transform) {
	m_transform = transform;
}

float& Camera::GetFOV() {
	return m_fov;
}

glm::mat4 Camera::GetViewMatrix() const {
	glm::mat4 view_matrix = glm::mat4(1.0f);
	view_matrix = glm::eulerAngleZXY(m_transform.rotation.z, m_transform.rotation.x, m_transform.rotation.y) * view_matrix;
	view_matrix = glm::translate(view_matrix, -m_transform.position);
	return view_matrix;
}

glm::mat4 Camera::GetProjectionMatrix() const {
	float aspect_ratio = (float)m_game->GetWindow()->GetWidth()/(float)m_game->GetWindow()->GetHeight();
	return glm::perspective(m_fov, aspect_ratio, 0.1f, 512.0f);
}
