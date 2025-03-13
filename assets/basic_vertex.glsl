#version 330 core

layout (location=0) in vec3 i_vertex_position;
layout (location=1) in vec3 i_vertex_normal;
layout (location=2) in vec2 i_texture_coord;

out vec3 normal;
noperspective out vec2 texture_coord;

uniform mat4 model_view_matrix;
uniform mat4 normal_matrix;
uniform mat4 projection_matrix;

void main() {
	vec2 snapsize = vec2(1920,1080)/16;
	vec4 position = projection_matrix * model_view_matrix * vec4(i_vertex_position, 1.0);
	vec3 ndc = position.xyz/position.w;
	ndc.xy = floor(ndc.xy * snapsize)/snapsize;
	ndc *= position.w;
	gl_Position = vec4(ndc, position.w);
	normal = normalize((normal_matrix * vec4(i_vertex_normal, 0.0)).xyz);
	texture_coord = i_texture_coord;
}
