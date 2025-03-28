#include "game_object.hpp"
#include "vec3.hpp"

struct game_object_data {
public:
	Transform transform;

	game_object_data() {
		transform = Transform{vec3(),vec3(),vec3(1.0f)};
	};
	game_object_data(vec3 position, vec3 rotation, vec3 scale) {
		transform = Transform{position,position,position};
	}
	game_object_data(Transform transform) {
		this->transform = transform;
	}

	void set_transform(Transform transform) {this->transform = transform;}

	Transform& get_transform() {return transform;}
};

GameObject::GameObject(std::string script_path, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform) {
	m_script_path = script_path;

	m_mesh = mesh;
	m_material = material;
	m_transform = transform;
	
	m_lua_state = sol::state();
	m_lua_state.open_libraries(sol::lib::base, sol::lib::math);

	m_lua_state.script_file(script_path, sol::load_mode::text);

	sol::usertype<vec3> vec3_data_type = m_lua_state.new_usertype<vec3>("Vector3",
		sol::constructors<vec3(), vec3(float), vec3(float,float,float)>());

	vec3_data_type["x"] = sol::property(&vec3::get_x, &vec3::set_x);
	vec3_data_type["y"] = sol::property(&vec3::get_y, &vec3::set_y);
	vec3_data_type["z"] = sol::property(&vec3::get_z, &vec3::set_z);

	sol::usertype<Transform> transform_data_type = m_lua_state.new_usertype<Transform>("Transform",
		sol::constructors<Transform(vec3,vec3,vec3)>());

	transform_data_type["position"] = sol::property(&Transform::get_position, &Transform::set_position);
	transform_data_type["rotation"] = sol::property(&Transform::get_rotation, &Transform::set_rotation);
	transform_data_type["scale"] = sol::property(&Transform::get_scale, &Transform::set_scale);
	
	sol::usertype<GameObject> game_object_data_type = m_lua_state.new_usertype<GameObject>("GameObject",
		sol::constructors<>());

	game_object_data_type["transform"] = sol::property(&GameObject::GetTransform, &GameObject::SetTransform);

	m_lua_state["_GameObject"] = this;

	m_lua_state["init"]();
	m_lua_process = m_lua_state["process"];
}

GameObject::~GameObject() {}

void GameObject::Process(float delta) {
	m_lua_process(delta);
	/*game_object_data result = m_lua_state["_GameObject"];
	m_transform = result.transform;*/
}

void GameObject::SetMesh(std::shared_ptr<Mesh> mesh) {m_mesh = mesh;}
void GameObject::SetMaterial(std::shared_ptr<Material> material) {m_material = material;}
void GameObject::SetTransform(const Transform& transform) {m_transform = transform;}

std::shared_ptr<Mesh> GameObject::GetMesh() {return m_mesh;}
std::shared_ptr<Material> GameObject::GetMaterial() {return m_material;}
Transform& GameObject::GetTransform() {return m_transform;}
