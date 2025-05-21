#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <vector>

struct event_connection {
	std::weak_ptr<sol::state> state;
	std::string function_name;
};

class Event {
public:
	Event();
	~Event();

	void Connect(std::shared_ptr<sol::state> state, std::string function_name);
	void Disconnect(std::shared_ptr<sol::state> state, std::string function_name);
	void Fire(sol::variadic_args va);

	int value = 5;
private:
	std::vector<event_connection> m_event_connections;
};
