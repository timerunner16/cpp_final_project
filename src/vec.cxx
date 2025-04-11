#include "vec3.hpp"
#include "ivec3.hpp"
#include "vec2.hpp"
#include "ivec2.hpp"

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
