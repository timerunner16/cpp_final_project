#include "event.hpp"

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

void Event::Fire(sol::variadic_args va) {
	std::erase_if(m_event_connections, [](const event_connection& connection) -> bool {
		return connection.state.expired();
	});
	for (auto connection : m_event_connections) {
		std::shared_ptr<sol::state> state = connection.state.lock();
		if (!state) continue;
		sol::protected_function_result result = state->get<sol::function>(connection.function_name)(va);
		if (!result.valid()) {
			sol::error error = result;
			printf("[LUA_ERROR]: %s\n", error.what());
		}
	}
}
