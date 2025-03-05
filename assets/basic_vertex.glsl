#version 330 core

layout (location=0) in vec3 i_vertex_position;
layout (location=1) in vec3 i_vertex_normal;
layout (location=2) in vec2 i_texture_coord;

out vec3 normal;
out vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(i_vertex_position, 1.0);
	normal = i_vertex_normal;
	texture_coord = i_texture_coord;
}
