#include "pdata_manager.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"
#include "particle_system.hpp"
#include "transform.hpp"
#include "physics.hpp"
#include "mesh.hpp"
#include "gltexture.hpp"
#include "shader.hpp"
#include "material.hpp"

PDataManager::PDataManager() {
	m_pdata_values = std::map<std::string, pdata_value>();
}

PDataManager::~PDataManager() {
	m_pdata_values.clear();
}

void PDataManager::SetPData(std::string key, pdata_value value) {
	m_pdata_values[key] = value;
}

pdata_value PDataManager::GetPData(std::string key) {
	if (m_pdata_values.contains(key)) return m_pdata_values[key];
	return sol::nil;
}
