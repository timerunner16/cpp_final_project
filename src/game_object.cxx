#include "game_object.hpp"
#include "game.hpp"
#include "lua_usertype_setup.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "map.hpp"
#include "workspace.hpp"
#include "event.hpp"
#include "audio_instance.hpp"
#include <algorithm>

#define FLOOR_EPSILON 0.005f

GameObject::GameObject(Game* game, std::string name, GameObject* parent,
		std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
		const Transform& transform, const Box& box, const float& height) {
	m_game = game;

	m_name = name;

	m_queued_for_freedom = false;

	m_parent = nullptr;
	SetParent(parent);

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;
	m_box = box;
	m_box.center = vec2{m_transform.position.x, m_transform.position.z};
	m_velocity = vec3{0,0,0};

	m_visible = true;

	m_height = height;

	m_events = std::map<std::string, Event*>();

	m_audio_instances = std::map<std::string, AudioInstance*>();

	m_lua_loaded = !script_path.empty();
	if (!m_lua_loaded) return;
	m_lua_state = std::make_shared<sol::state>();
	m_lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

	lua_usertype_setup(m_game, m_lua_state, this);

	m_lua_state->script_file(script_path, sol::load_mode::text);

	sol::safe_function init = m_lua_state->get<sol::function>("init");
	if (init.valid()) {
		sol::protected_function_result result = init();
		if (!result.valid()) {
			sol::error error = result;
			printf("[LUA ERROR]: %s\n", error.what());
		}
	}

	m_lua_process = m_lua_state->get<sol::function>("process");
	m_lua_on_destruct = m_lua_state->get<sol::function>("on_destruct");
}

GameObject::~GameObject() {
	if (m_parent != nullptr) m_parent->RemoveChild(m_name);
	for (auto& [key, val] : m_events) {delete val;}
	for (auto& [key, val] : m_children) {delete val;}
}

void GameObject::Process(float delta) {
	if (m_lua_loaded) {
		if (m_lua_process.valid()) {
			sol::protected_function_result result = m_lua_process(delta);
			if (!result.valid()) {
				sol::error error = result;
				printf("[LUA_ERROR]: %s\n", error.what());
			}
		}
	}

	m_box.center = vec2{m_transform.position.x, m_transform.position.z};
	vec2 in_velocity = vec2{m_velocity.x, m_velocity.z} * delta;

	if (m_box.bound.x > EPSILON && m_box.bound.y > EPSILON) {
		std::vector<vec2> forces(0);
		float min = m_transform.position.y;
		float max = m_transform.position.y + m_height;
		for (line segment : m_game->GetMap()->GetLines()) {
			if ((max > segment.floor && min < segment.midfloor && segment.lower && !(segment.midfloor - min < STEP_SIZE)) ||
				(max > segment.midfloor && min < segment.midceiling && segment.mid) ||
				(max > segment.midceiling && min < segment.ceiling && segment.higher)) {
				collision_result result = sweep_box_line(m_box, segment, in_velocity);
				if (result.hit) {
					forces.push_back(result.until_blocked + result.out_velocity - in_velocity);
				}
			}
		}
		for (auto& [key, val] : m_game->GetWorkspace()->GetGameObjects()) {
			std::vector<collision_result> results = val->CollideBox(m_box, in_velocity);
			for (collision_result result : results)
				if (result.hit) forces.push_back(result.until_blocked);
		}
		for (vec2 force : forces) {
			m_transform.position.x += force.x;
			m_transform.position.z += force.y;
		}
	}
	
	m_transform.position.x += in_velocity.x;
	m_transform.position.y += m_velocity.y * delta;
	m_transform.position.z += in_velocity.y;
	m_box.center = vec2{m_transform.position.x, m_transform.position.z};

	std::optional<Sector> highest_floor_sector = m_game->GetMap()->GetHighestFloorOverlapping(m_box);
	if (highest_floor_sector.has_value()) {
		if (m_transform.position.y < highest_floor_sector->heightfloor) m_transform.position.y = highest_floor_sector->heightfloor;
		if (abs(m_transform.position.y-highest_floor_sector->heightfloor) < FLOOR_EPSILON) m_on_floor = true;
		else m_on_floor = false;
	}
	std::optional<Sector> lowest_ceiling_sector = m_game->GetMap()->GetHighestFloorOverlapping(m_box);
	if (lowest_ceiling_sector.has_value()) {
		if (m_transform.position.y + m_height > lowest_ceiling_sector->heightceiling) m_transform.position.y = lowest_ceiling_sector->heightceiling-m_height;
		if (abs(m_transform.position.y+m_height-lowest_ceiling_sector->heightceiling) < FLOOR_EPSILON) m_on_ceiling = true;
		else m_on_ceiling = false;
	}

	for (auto& [key, val] : m_children) {val->Process(delta);}
	std::erase_if(m_children, [](const auto& item) -> bool {
		const auto& [key, val] = item;
		if (val->IsQueuedForFreedom()) {
			delete val;
			return true;
		}
		return false;
	});
}

std::vector<collision_result> GameObject::CollideBox(const Box& moving, vec2 v) {
	if (moving.center == m_box.center) return std::vector<collision_result>(0);
	std::vector<collision_result> outputs(0);
	for (auto& [key, val] : m_children) {
		std::vector<collision_result> child_outputs(0);
		child_outputs = val->CollideBox(moving, v);
		for (collision_result i : child_outputs) outputs.push_back(i);
	}
	if (m_box.bound != vec2()) outputs.push_back(discrete_box_box(moving, m_box, v));
	return outputs;
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}
void GameObject::SetBox(const Box& box) {m_box = box;}
void GameObject::SetVelocity(const vec3& velocity) {m_velocity = velocity;}
void GameObject::SetHeight(const float& height) {m_height = height;}

std::string GameObject::GetName() {return m_name;}
std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
Box& GameObject::GetBox() {return m_box;}
Transform GameObject::GetGlobalTransform() {
	glm::mat4 model_matrix = m_transform.GetModelMatrix();
	GameObject* parent = m_parent;
	while (parent != nullptr) {
		glm::mat4 parent_matrix = parent->GetTransform().GetModelMatrix();
		model_matrix = parent_matrix * model_matrix;
		parent = parent->GetParent();
	}
	vec3 position;
	vec3 scale;
	vec3 skew;
	glm::vec4 perspective;
	glm::quat orientation;
	glm::decompose(model_matrix, scale, orientation, position, skew, perspective);
	vec3 rotation = glm::eulerAngles(orientation);	
	return Transform{position, rotation, scale};
}
vec3& GameObject::GetVelocity() {return m_velocity;}
float GameObject::GetHeight() {return m_height;}

GameObject* GameObject::GetParent() {return m_parent;}
GameObject* GameObject::GetChild(std::string name) {
	if (m_children.contains(name)) return m_children[name];
	return nullptr;
}
std::map<std::string, GameObject*> GameObject::GetChildren() {return m_children;}
sol::as_table_t<std::map<std::string,GameObject*>> GameObject::GetChildren_Lua() {
	return (sol::as_table_t<std::map<std::string, GameObject*>>)(m_children);
}

void GameObject::SetParent(GameObject* parent) {
	if (m_parent != nullptr) {
		m_parent->RemoveChild(m_name);
	}
	if (parent != nullptr) {
		parent->AddChild(this);
	}
	m_parent = parent;
}

void GameObject::RemoveChild(std::string name) {
	if (m_children.contains(name)) m_children.erase(name);
}

void GameObject::AddChild(GameObject* child) {
	if (child == nullptr) return;
	m_children[child->GetName()] = child;
}

Event* GameObject::GetEvent(std::string name) {
	if (m_events.contains(name)) return m_events[name];
	return nullptr;
}
void GameObject::RemoveEvent(std::string name) {
	if (m_events.contains(name)) {
		delete m_events[name];
		m_events.erase(name);
	}
}
void GameObject::AddEvent(Event* event, std::string name) {
	if (event == nullptr) return;
	m_events[name] = event;
}

bool GameObject::IsOnFloor() {return m_on_floor;}
bool GameObject::IsOnCeiling() {return m_on_ceiling;}

collision_result GameObject::Raycast(vec3 origin, vec2 endpoint, std::vector<GameObject*> filter) {
	std::vector<collision_result> results(0);
	vec2 origin2d{origin.x, origin.z};
	line ray{origin2d, endpoint};
	for (line segment : m_game->GetMap()->GetLines()) {
		if ((origin.y > segment.floor && origin.y < segment.midfloor && segment.lower) ||
			(origin.y > segment.midfloor && origin.y < segment.midceiling && segment.mid) ||
			(origin.y > segment.midceiling && origin.y < segment.ceiling && segment.higher)) {
			collision_result result = discrete_line_line(ray, segment);
			if (result.hit) results.push_back(result);
		}
	}
	for (auto& [key, val] : m_game->GetWorkspace()->GetGameObjects()) {
		collision_result result = val->RaycastBox(filter, ray);
		result.game_object = val;
		if (result.hit) results.push_back(result);
	}

	if (results.size() == 0) return collision_result{false, vec2(), vec2(), vec2(), nullptr};

	collision_result shortest = results[0];
	float distance = vec2(shortest.until_blocked - origin2d).length_squared();
	for (size_t i = 0; i < results.size(); i++) {
		float new_distance = vec2(results[i].until_blocked - origin2d).length_squared();
		if (new_distance < distance) {
			shortest = results[i];
			distance = new_distance;
		}
	}
	return shortest;
}

collision_result GameObject::RaycastBox(std::vector<GameObject*> filter, line ray) {
	std::vector<collision_result> results(0);
	if (std::find(filter.begin(), filter.end(), this) == filter.end()) {
		for (auto& [key, val] : m_children) {
			collision_result result = val->RaycastBox(filter, ray);
			if (result.hit) results.push_back(result);
		}
		if (m_box.bound != vec2()) {
			collision_result result = discrete_line_box(m_box, ray);
			if (result.hit) results.push_back(result);
		}
	}
	if (results.size() == 0) return collision_result{false, vec2(), vec2(), vec2()};

	collision_result shortest = results[0];
	float distance = vec2(shortest.until_blocked - ray.a).length_squared();
	for (size_t i = 0; i < results.size(); i++) {
		float new_distance = vec2(results[i].until_blocked - ray.a).length_squared();
		if (new_distance < distance) {
			shortest = results[i];
			distance = new_distance;
		}
	}
	return shortest;
}

void GameObject::QueueFree() {
	if (m_lua_on_destruct.valid()) {
		sol::protected_function_result result = m_lua_on_destruct();
		if (!result.valid()) {
			sol::error error = result;
			printf("[LUA ERROR]: %s\n", error.what());
		}
	}

	m_queued_for_freedom = true;
}
bool GameObject::IsQueuedForFreedom() {return m_queued_for_freedom;}

void GameObject::SetUniform(Uniform uniform) {
	if (m_uniforms.contains(uniform.name)) RemoveUniform(uniform.name);
	size_t size = get_uniform_data_size(uniform.type);
	void* data = malloc(size);
	memcpy(data, uniform.data, size);
	Uniform n_uniform{uniform.name, uniform.type, data};
	m_uniforms[uniform.name] = n_uniform;
}

std::map<std::string, Uniform> GameObject::GetUniforms() {
	return m_uniforms;
}

void GameObject::RemoveUniform(std::string name) {
	if (!m_uniforms.contains(name)) return;
	free(m_uniforms[name].data);
	m_uniforms.erase(name);
}

void GameObject::SetVisible(bool visible) {m_visible = visible;}

bool GameObject::GetVisible() {return m_visible;}

AudioInstance* GameObject::GetAudioInstance(std::string name) {
	if (m_audio_instances.contains(name)) return m_audio_instances[name];
	return nullptr;
}
void GameObject::RemoveAudioInstance(std::string name) {
	if (m_audio_instances.contains(name)) {
		delete m_audio_instances[name];
		m_audio_instances.erase(name);
	}
}
void GameObject::AddAudioInstance(AudioInstance* audio_instance, std::string name) {
	if (audio_instance == nullptr) return;
	audio_instance->SetParent(this);
	m_audio_instances[name] = audio_instance;
}
