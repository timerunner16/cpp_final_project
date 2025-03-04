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

float& Camera::GetFOV() {
	return m_fov;
}

glm::mat4 Camera::GetViewMatrix() const {
	glm::mat4 view_matrix = glm::mat4(1.0f);
	view_matrix = glm::translate(view_matrix, m_transform.position);
	view_matrix *= glm::mat4_cast(glm::quat(m_transform.rotation));
	return view_matrix;
}

glm::mat4 Camera::GetProjectionMatrix() const {
	float aspect_ratio = (float)m_game->GetWindow()->GetWidth()/(float)m_game->GetWindow()->GetHeight();
	return glm::perspective(m_fov, aspect_ratio, 0.5f, 512.0f);
}
