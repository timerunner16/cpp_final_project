#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "ivec2.hpp"
#include "ivec3.hpp"

vec2::vec2(const glm::vec2& obj) {
	this->x = obj.x;
	this->y = obj.y;
}
vec2::vec2(const ivec2& obj) {
	this->x = (float)obj.x;
	this->y = (float)obj.y;
}

void vec2::set_x(float x) {this->x = x;}
void vec2::set_y(float y) {this->y = y;}

float vec2::get_x() {return x;}
float vec2::get_y() {return y;}

vec2 vec2::add(const vec2& other) {return vec2(x+other.x, y+other.y);}
vec2 vec2::sub(const vec2& other) {return vec2(x-other.x, y-other.y);}
vec2 vec2::mul(const vec2& other) {return vec2(x*other.x, y*other.y);}
vec2 vec2::div(const vec2& other) {return vec2(x/other.x, y/other.y);}

vec2 vec2::mulf(const float& other) {return vec2(x*other, y*other);}
vec2 vec2::divf(const float& other) {return vec2(x/other, y/other);}

vec2 vec2::unary_minus() {return vec2(-x, -y);}

float vec2::length() const {return sqrt(x*x + y*y);}
float vec2::length_squared() const {return x*x + y*y;}
float vec2::dot(const vec2& other) const {return x*other.x + y*other.y;}
vec2 vec2::unit() const {
	float magnitude = length();
	if (magnitude == 0) return vec2(0.0f);
	return vec2(x,y).divf(magnitude);
}

vec2 vec2::withX(float x) {return vec2(x, this->y);}
vec2 vec2::withY(float y) {return vec2(this->x, y);}


vec3::vec3(const glm::vec3& obj) {
	this->x = obj.x;
	this->y = obj.y;
	this->z = obj.z;
}
vec3::vec3(const ivec3& obj) {
	this->x = (float)obj.x;
	this->y = (float)obj.y;
	this->z = (float)obj.z;
}

void vec3::set_x(float x) {this->x = x;}
void vec3::set_y(float y) {this->y = y;}
void vec3::set_z(float z) {this->z =z;}

float vec3::get_x() {return x;}
float vec3::get_y() {return y;}
float vec3::get_z() {return z;}

vec3 vec3::add(const vec3& other) {return vec3(x+other.x, y+other.y, z+other.z);}
vec3 vec3::sub(const vec3& other) {return vec3(x-other.x, y-other.y, z-other.z);}
vec3 vec3::mul(const vec3& other) {return vec3(x*other.x, y*other.y, z*other.z);}
vec3 vec3::div(const vec3& other) {return vec3(x/other.x, y/other.y, z/other.z);}

vec3 vec3::mulf(const float& other) {return vec3(x*other, y*other, z*other);}
vec3 vec3::divf(const float& other) {return vec3(x/other, y/other, z/other);}

vec3 vec3::unary_minus() {return vec3(-x, -y, -z);}

float vec3::length() const {return sqrt(x*x + y*y + z*z);}
float vec3::length_squared() const {return x*x + y*y + z*z;}
float vec3::dot(const vec3& other) const {return x*other.x + y*other.y + z*other.z;}
vec3 vec3::unit() const {
	float magnitude = length();
	if (magnitude == 0) return vec3(0.0f);
	return vec3(x,y,z).divf(magnitude);
}
vec3 vec3::cross(const vec3& other) const {
	return glm::cross(*this, other);
}

vec3 vec3::withX(float x) {return vec3(x, this->y, this->z);}
vec3 vec3::withY(float y) {return vec3(this->x, y, this->z);}
vec3 vec3::withZ(float z) {return vec3(this->x, this->y, z);}


vec4::vec4(const glm::vec4& obj) {
	this->x = obj.x;
	this->y = obj.y;
	this->z = obj.z;
}

void vec4::set_x(float x) {this->x = x;}
void vec4::set_y(float y) {this->y = y;}
void vec4::set_z(float z) {this->z = z;}
void vec4::set_w(float w) {this->w = w;}

float vec4::get_x() {return x;}
float vec4::get_y() {return y;}
float vec4::get_z() {return z;}
float vec4::get_w() {return w;}


ivec2::ivec2(const glm::ivec2& obj) {
	this->x = obj.x;
	this->y = obj.y;
}
ivec2::ivec2(const vec2& obj) {
	this->x = (int)obj.x;
	this->y = (int)obj.y;
}

void ivec2::set_x(int x) {this->x = x;}
void ivec2::set_y(int y) {this->y = y;}

int ivec2::get_x() {return x;}
int ivec2::get_y() {return y;}

ivec2 ivec2::add(const ivec2& other) {return ivec2(x+other.x, y+other.y);}
ivec2 ivec2::sub(const ivec2& other) {return ivec2(x-other.x, y-other.y);}
ivec2 ivec2::mul(const ivec2& other) {return ivec2(x*other.x, y*other.y);}
ivec2 ivec2::div(const ivec2& other) {return ivec2(x/other.x, y/other.y);}

ivec2 ivec2::mulf(const float& other) {return ivec2((int)(x*other), (int)(y*other));}
ivec2 ivec2::divf(const float& other) {return ivec2((int)(x/other), (int)(y/other));}

ivec2 ivec2::unary_minus() {return ivec2(-x, -y);}

float ivec2::length() const {return sqrt(x*x + y*y);}
float ivec2::length_squared() const {return x*x + y*y;}

ivec2 ivec2::withX(int x) {return ivec2(x, this->y);}
ivec2 ivec2::withY(int y) {return ivec2(this->x, y);}


ivec3::ivec3(const glm::ivec3& obj) {
	this->x = obj.x;
	this->y = obj.y;
	this->z = obj.z;
}
ivec3::ivec3(const vec3& obj) {
	this->x = (int)obj.x;
	this->y = (int)obj.y;
	this->z = (int)obj.z;
}

void ivec3::set_x(int x) {this->x = x;}
void ivec3::set_y(int y) {this->y = y;}
void ivec3::set_z(int z) {this->z = z;}

int ivec3::get_x() {return x;}
int ivec3::get_y() {return y;}
int ivec3::get_z() {return z;}

ivec3 ivec3::add(const ivec3& other) {return ivec3(x+other.x, y+other.y, z+other.z);}
ivec3 ivec3::sub(const ivec3& other) {return ivec3(x-other.x, y-other.y, z-other.z);}
ivec3 ivec3::mul(const ivec3& other) {return ivec3(x*other.x, y*other.y, z*other.z);}
ivec3 ivec3::div(const ivec3& other) {return ivec3(x/other.x, y/other.y, z/other.z);}

ivec3 ivec3::mulf(const float& other) {return ivec3((int)(x*other), (int)(y*other), (int)(z*other));}
ivec3 ivec3::divf(const float& other) {return ivec3((int)(x/other), (int)(y/other), (int)(z/other));}

ivec3 ivec3::unary_minus() {return ivec3(-x, -y, -z);}

float ivec3::length() const {return sqrt(x*x + y*y + z*z);}
float ivec3::length_squared() const {return x*x + y*y + z*z;}

ivec3 ivec3::withX(int x) {return ivec3(x, this->y, this->z);}
ivec3 ivec3::withY(int y) {return ivec3(this->x, y, this->z);}
ivec3 ivec3::withZ(int z) {return ivec3(this->x, this->y, z);}
