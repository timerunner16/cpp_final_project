#pragma once
#include <GL/glew.h>
#include <GL/glu.h>
#include <memory>
#include <vector>
#include "vec2.hpp"
#include "vec3.hpp"

class Material;

struct particle_system_create_info {
	vec3 position=vec3(0,0,0);
	vec3 direction=vec3(0,1,0);
	vec2 randomization=vec2(M_PI_4,M_PI_4);
	vec2 size=vec2(0.5f,0.5f);
	float speed=10;
	float gravity=-9.81f;
	float lifetime=10;
	uint8_t r=255;
	uint8_t g=255;
	uint8_t b=255;
	uint8_t a=255;
	uint8_t num_particles=64;
};

struct particle {
	vec3 position;
	vec3 velocity;
};

class ParticleSystem {
public:
	ParticleSystem(particle_system_create_info info);
	~ParticleSystem();

	void Update(float delta);
	std::shared_ptr<Material> GetMaterial();
	GLuint GetVAO();
	size_t GetInstances();
	bool GetDead();
private:
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<particle> m_particles;
	float m_gravity;
	float m_remaining_life;
	std::shared_ptr<Material> m_material;
	glm::vec4 m_color;
};
