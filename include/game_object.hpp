#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <memory>
#include <string>
#include <map>
#include "transform.hpp"
#include "physics.hpp"
#include "material.hpp"

class Game;
class Event;
class Mesh;
class Material;

class GameObject {
public:
	GameObject(Game* game, std::string name, GameObject* parent,
			std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
			const Transform& transform, const Box& box, const float& height);
	~GameObject();

	void Process(float delta);

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetTransform(const Transform& transform);
	void SetBox(const Box& box);
	void SetVelocity(const vec3& velocity);
	void SetHeight(const float& height);

	std::string GetName();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform& GetTransform();
	Box& GetBox();
	Transform GetGlobalTransform();
	vec3& GetVelocity();
	float GetHeight();

	std::vector<collision_result> CollideBox(const Box& moving, vec2 v);

	GameObject* GetParent();
	GameObject* GetChild(std::string name);
	std::map<std::string, GameObject*> GetChildren();
	sol::as_table_t<std::map<std::string, GameObject*>> GetChildren_Lua();
	void SetParent(GameObject*);
	void RemoveChild(std::string name);
	void AddChild(GameObject* child);

	Event* GetEvent(std::string name);
	void RemoveEvent(std::string name);
	void AddEvent(Event* event, std::string name);
	
	bool IsOnFloor();

	collision_result Raycast(vec3 origin, vec2 endpoint, std::vector<GameObject*> filter);
	collision_result RaycastBox(std::vector<GameObject*> filter, line l);

	void QueueFree();
	bool IsQueuedForFreedom();

	void SetUniform(Uniform uniform);
	std::map<std::string, Uniform> GetUniforms();
private:
	Game* m_game;

	std::string m_name;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	Transform m_transform;
	Box m_box;
	float m_height;
	vec3 m_velocity;

	bool m_lua_loaded;
	std::shared_ptr<sol::state> m_lua_state;
	sol::safe_function m_lua_process;

	GameObject* m_parent;
	std::map<std::string, GameObject*> m_children;

	std::map<std::string, Event*> m_events;

	bool m_on_floor;

	bool m_queued_for_freedom;

	std::map<std::string, Uniform> m_uniforms;
};
