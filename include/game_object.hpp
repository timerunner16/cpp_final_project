#pragma once
#include <string>

class Mesh;
class Shader;

class GameObject {
public:
	GameObject(std::string script_path);
	~GameObject();

	void Process();

	void SetMesh(Mesh* mesh);
	void SetShader(Shader* shader);

	Mesh* GetMesh();
	Shader* GetShader();
private:
	std::string m_script_path;
	Mesh* m_mesh;
	Shader* m_shader;
};
