#version 330 core

in vec3 normal;
in vec2 texture_coord;

out vec4 fragment;

uniform sampler2D texture0;

void main() {
	float lightval = max(dot(normalize(normal), normalize(vec3(1.0, 0.7, 0.8))),0.0);
	fragment = texture(texture0, texture_coord);
	fragment.xyz *= (lightval*0.8+0.2);
}
