#pragma once
#include <string>

class GameObject {
public:
	GameObject(std::string script_path);
	~GameObject();

	void Process();
private:
	std::string m_script_path;
};
