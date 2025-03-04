#pragma once
#include <string>
#include "transform.hpp"

class Mesh;
class Shader;

class GameObject {
public:
	GameObject(std::string script_path);
	~GameObject();

	void Process();

	void SetMesh(Mesh* mesh);
	void SetShader(Shader* shader);
	void SetTransform(const Transform& transform);

	Mesh* GetMesh();
	Shader* GetShader();
	Transform& GetTransform();
private:
	std::string m_script_path;
	Mesh* m_mesh;
	Shader* m_shader;
	Transform m_transform;
};
