#pragma once
#include <map>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <vector>

class vec2;
class vec3;
class ivec2;
class ivec3;
class particle_system_create_info;
class Transform;
class Box;
class Event;
class GameObject;
class Camera;
class Mesh;
class GLTexture;
class Shader;
class Material;
class Uniform;

typedef std::variant<
	int, bool, float,
	vec2, vec3, ivec2, ivec3,
	particle_system_create_info,
	Transform, Box,
	Event, GameObject, Camera,
	Mesh, GLTexture, Shader, Material, Uniform,
	sol::nil_t
> event_value;

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
	void Fire();

	void SetValue(std::string name, event_value value);
	event_value GetValue(std::string name);
private:
	std::vector<event_connection> m_event_connections;
	std::map<std::string, event_value> m_event_values;
};
