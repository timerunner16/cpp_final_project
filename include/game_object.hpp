#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <memory>
#include <string>
#include "transform.hpp"
#include "mesh.hpp"
#include "material.hpp"

class Game;

class GameObject {
public:
	GameObject(Game* game, std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform);
	~GameObject();

	void Process(float delta);

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetTransform(const Transform& transform);

	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform& GetTransform();
private:
	Game* m_game;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	Transform m_transform;

	sol::state m_lua_state;
	sol::function m_lua_process;
};
