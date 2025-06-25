#pragma once
#include <memory>
#include <vector>
#include "glcommon.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

class Material;

struct particle_system_create_info {
	vec3 position=vec3(0,0,0);
	vec3 direction=vec3(0,1,0);
	vec2 size=vec2(0.5f,0.5f);
	float randomization=1;
	float speed=10;
	float gravity=-9.81f;
	float lifetime=1;
	uint8_t num_particles=8;
	uint8_t num_launches=8;
	float launch_interval = 1;
	uint8_t r=255;
	uint8_t g=255;
	uint8_t b=255;
	uint8_t a=255;
	bool fadeout=false;
};

struct particle {
	vec3 position;
	vec3 velocity;
	float remaining_life;
};

class ParticleSystem {
public:
	ParticleSystem(particle_system_create_info info);
	~ParticleSystem();

	void Update(float delta);
	void AddParticles();
	std::shared_ptr<Material> GetMaterial();
	GLuint GetVAO();
	size_t GetInstances();
	bool GetDead();
private:
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<particle> m_particles;
	float m_gravity;
	float m_speed;
	float m_lifetime;
	float m_remaining_life;
	float m_last_launch;
	uint8_t m_remaining_launches;
	uint8_t m_particles_per_launch;
	float m_launch_interval;
	std::shared_ptr<Material> m_material;
	glm::vec4 m_color;
	float m_randomization;
	vec3 m_position;
	vec3 m_direction;
	bool m_fadeout;
};
