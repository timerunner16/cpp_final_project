#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <memory>
#include <string>
#include <map>
#include "transform.hpp"
#include "mesh.hpp"
#include "material.hpp"

class Game;

class GameObject {
public:
	GameObject(Game* game, std::string name, GameObject* parent,
			std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
			const Transform& transform);
	~GameObject();

	void Process(float delta);

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetTransform(const Transform& transform);

	std::string GetName();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform& GetTransform();

	GameObject* GetParent();
	GameObject* GetChild(std::string name);

	void SetParent(GameObject*);

	void RemoveChild(std::string name);
	void AddChild(GameObject* child);
private:
	Game* m_game;

	std::string m_name;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	Transform m_transform;

	bool m_lua_loaded;
	sol::state m_lua_state;
	sol::safe_function m_lua_process;

	GameObject* m_parent;
	std::map<std::string, GameObject*> m_children;
};
