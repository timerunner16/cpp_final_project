#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <memory>
#include <string>
#include <map>
#include "transform.hpp"
#include "physics.hpp"

class Game;
class Event;
class Mesh;
class Material;

class GameObject {
public:
	GameObject(Game* game, std::string name, GameObject* parent,
			std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
			const Transform& transform, const Box& box);
	~GameObject();

	void Process(float delta);

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetTransform(const Transform& transform);
	void SetBox(const Box& box);
	void SetVelocity(const vec3& velocity);

	std::string GetName();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform& GetTransform();
	Box& GetBox();
	Transform GetGlobalTransform();
	vec3& GetVelocity();

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
private:
	Game* m_game;

	std::string m_name;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	Transform m_transform;
	Box m_box;
	vec3 m_velocity;

	bool m_lua_loaded;
	std::shared_ptr<sol::state> m_lua_state;
	sol::safe_function m_lua_process;

	GameObject* m_parent;
	std::map<std::string, GameObject*> m_children;

	std::map<std::string, Event*> m_events;
};
