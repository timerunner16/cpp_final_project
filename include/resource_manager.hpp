#pragma once
#include <string>
#include <map>
#include <memory>
#include <type_traits>

class Game;

class GenericResource {
public:
	~GenericResource() {Cleanup();};

	virtual void Cleanup() {}

	void SetPersistent(bool persistent);

	std::string GetType() {return m_type_str;}
	bool GetPersistent() {return m_persistent;}
protected:
	std::string m_type_str;
private:
	bool m_persistent;
};

class ResourceManager {
public:
	ResourceManager(Game* game);
	~ResourceManager();

	void ClearUnusedResources();

	template <class T>
	typename std::enable_if_t<std::constructible_from<T, Game*, std::string> && std::is_base_of_v<GenericResource, T>, std::shared_ptr<T>> GetResource(std::string file_path);
	
	template <class T>
	typename std::enable_if_t<std::constructible_from<T, std::string> && std::is_base_of_v<GenericResource, T>, std::shared_ptr<T>> GetResource(std::string file_path);
private:
	std::map<std::string, std::shared_ptr<GenericResource>> m_resource_map;
	Game* m_game;
};
