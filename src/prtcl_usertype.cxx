#include "prtcl_usertype.hpp"
#include "particle_system.hpp"

void prtcl_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject *game_object) {
	sol::usertype<particle_system_create_info> particle_system_create_info_data_type = lua_state->new_usertype<particle_system_create_info>(
		"ParticleSystemCreateInfo",
		sol::factories(
			[]() {
				return new particle_system_create_info{};
			}
		),
		"Position", &particle_system_create_info::position,
		"Direction", &particle_system_create_info::direction,
		"Randomization", &particle_system_create_info::randomization,
		"Size", &particle_system_create_info::size,
		"Speed", &particle_system_create_info::speed,
		"Gravity", &particle_system_create_info::gravity,
		"Lifetime", &particle_system_create_info::lifetime,
		"NumParticles", &particle_system_create_info::num_particles,
		"NumLaunches", &particle_system_create_info::num_launches,
		"LaunchInterval", &particle_system_create_info::launch_interval,
		"R", &particle_system_create_info::r,
		"G", &particle_system_create_info::g,
		"B", &particle_system_create_info::b,
		"A", &particle_system_create_info::a,
		"FadeOut", &particle_system_create_info::fadeout
	);
}
