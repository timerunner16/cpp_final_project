#pragma once
#include <memory>
#include <string>
#include "transform.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"

class Mesh;
class Shader;

class GameObject {
public:
	GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, Shader* shader,
			std::shared_ptr<GLTexture> gl_texture, const Transform& transform);
	~GameObject();

	void Process();

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetShader(Shader* shader);
	void SetGLTexture(std::shared_ptr<GLTexture> gl_texture);
	void SetTransform(const Transform& transform);

	std::shared_ptr<Mesh> GetMesh();
	Shader* GetShader();
	std::shared_ptr<GLTexture> GetGLTexture();
	Transform& GetTransform();
private:
	std::string m_script_path;
	std::shared_ptr<Mesh> m_mesh;
	Shader* m_shader;
	std::shared_ptr<GLTexture> m_gl_texture;
	Transform m_transform;
};
