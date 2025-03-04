#pragma once
#include <glm/ext.hpp>
#include "transform.hpp"

class Game;

class Camera {
public:
	Camera(Game* game, const Transform& viewpoint, const float& fov);

	float& GetFOV();
	Transform& GetTransform();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
private:
	Game* m_game;
	float m_fov;
	Transform m_transform;
};
