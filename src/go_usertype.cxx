#include "go_usertype.hpp"
#include "game_object.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "transform.hpp"
#include "particle_system.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "event.hpp"

void go_usertype_setup(std::shared_ptr<sol::state> lua_state, GameObject* game_object) {
	sol::usertype<GameObject> game_object_data_type = lua_state->new_usertype<GameObject>(
		"GameObject", sol::no_constructor,
		"Transform", sol::property(&GameObject::GetTransform, &GameObject::SetTransform),
		"Velocity", sol::property(&GameObject::GetVelocity, &GameObject::SetVelocity),
		"GlobalTransform", sol::readonly_property(&GameObject::GetGlobalTransform),
		"GetChild", &GameObject::GetChild,
		"GetChildren", &GameObject::GetChildren_Lua,
		"GetParent", &GameObject::GetParent,
		"GetName", &GameObject::GetName,
		"SetMesh", &GameObject::SetMesh,
		"SetMaterial", &GameObject::SetMaterial,
		"GetEvent", &GameObject::GetEvent,
		"IsOnFloor", &GameObject::IsOnFloor,
		"Raycast", &GameObject::Raycast
	);


	sol::usertype<Camera> camera_data_type = lua_state->new_usertype<Camera>(
		"Camera", sol::no_constructor,
		"Transform", sol::property(&Camera::GetTransform, &Camera::SetTransform)
	);

	
	sol::usertype<Event> event_data_type = lua_state->new_usertype<Event>(
		"Event", sol::no_constructor,
		"Connect", [lua_state](Event& event, std::string function_name) -> void {
			event.Connect(lua_state, function_name);
		},
		"Disconnect", [lua_state](Event& event, std::string function_name) -> void {
			event.Disconnect(lua_state, function_name);
		},
		"Fire", &Event::Fire,
		"SetValue", &Event::SetValue,
		"GetValue", &Event::GetValue
	);
}
