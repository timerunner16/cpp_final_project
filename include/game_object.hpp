#pragma once
#include <memory>
#include <string>
#include "transform.hpp"
#include "mesh.hpp"
#include "material.hpp"

class GameObject {
public:
	GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform);
	~GameObject();

	void Process();

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	void SetTransform(const Transform& transform);

	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform& GetTransform();
private:
	std::string m_script_path;
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	Transform m_transform;
};
