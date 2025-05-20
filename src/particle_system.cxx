#include "particle_system.hpp"
#include "shader.hpp"
#include "material.hpp"
#include "util.hpp"
#include "builtin_particle_shader.h"

ParticleSystem::ParticleSystem(particle_system_create_info info) {
	m_gravity = info.gravity;
	m_remaining_life = info.lifetime;
	m_particles = std::vector<particle>(0);

	glm::quat q = glm::quatLookAt(info.direction, glm::vec3(0,1,0));
	glm::vec3 euler_base = glm::eulerAngles(q);

	for (size_t i = 0; i < info.num_particles; i++) {
		glm::vec3 euler = euler_base;
		euler.x += glm::linearRand(-info.randomization.x, info.randomization.x);
		euler.y += glm::linearRand(-info.randomization.y, info.randomization.y);
		vec3 direction{
		   -glm::cos(euler.x) * glm::sin(euler.y),
			glm::sin(euler.x),
	    	glm::cos(euler.x) * glm::cos(euler.y),
		};

		vec3 position = info.position;
		vec3 velocity = direction.unit() * info.speed;
		m_particles.push_back(particle{
			info.position, velocity
		});
	}
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glm::vec2 vertex_data[6] {
		{-info.size.x, -info.size.y},
		{ info.size.x, -info.size.y},
		{ info.size.x,  info.size.y},
		{-info.size.x, -info.size.y},
		{ info.size.x,  info.size.y},
		{-info.size.x,  info.size.y},
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

	m_material = std::make_shared<Material>(std::make_shared<Shader>((uint8_t*)particle_shader_source.c_str(), (uint32_t)particle_shader_source.size()), nullptr);
	m_color = glm::vec4{(float)info.r/255.0f, (float)info.g/255.0f, (float)info.b/255.0f, (float)info.a/255.0f};
	m_material->SetUniform({"color", VEC4, &m_color});

	Update(0);
}

ParticleSystem::~ParticleSystem() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void ParticleSystem::Update(float delta) {
	for (auto& i : m_particles) {
		i.velocity.y += delta * m_gravity;
		i.position += i.velocity * delta;
	}
	for (size_t i = 0; i < m_particles.size(); i++)
		m_material->SetUniform({"offsets[" + std::to_string(i) + "]", VEC3, &m_particles[i].position});
	m_remaining_life -= delta;
}

std::shared_ptr<Material> ParticleSystem::GetMaterial() {return m_material;}
GLuint ParticleSystem::GetVAO() {return m_vao;}
size_t ParticleSystem::GetInstances() {return m_particles.size();}
bool ParticleSystem::GetDead() {return m_remaining_life <= 0;}
