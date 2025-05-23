#include "event.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "particle_system.hpp"
#include "transform.hpp"
#include "physics.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "material.hpp"

Event::Event() {
	m_event_connections = std::vector<event_connection>(0);
}

Event::~Event() {
	m_event_connections.clear();
}

void Event::Connect(std::shared_ptr<sol::state> state, std::string function_name) {
	m_event_connections.push_back({std::weak_ptr<sol::state>(state), function_name});
}

void Event::Disconnect(std::shared_ptr<sol::state> state, std::string function_name) {
	std::erase_if(m_event_connections, [&state](const event_connection& connection) -> bool {
		return connection.state.expired() || connection.state.lock() == state;
	});
}

void Event::Fire() {
	std::erase_if(m_event_connections, [](const event_connection& connection) -> bool {
		return connection.state.expired();
	});
	for (auto connection : m_event_connections) {
		std::shared_ptr<sol::state> state = connection.state.lock();
		if (!state) continue;
		sol::protected_function_result result = state->get<sol::function>(connection.function_name)();
		if (!result.valid()) {
			sol::error error = result;
			printf("[LUA_ERROR]: %s\n", error.what());
		}
	}
}

void Event::SetValue(std::string name, event_value value) {
	m_event_values[name] = value;
}

event_value Event::GetValue(std::string name) {
	if (m_event_values.contains(name)) return m_event_values[name];
	return sol::nil;
}
