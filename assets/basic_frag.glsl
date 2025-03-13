#version 330 core

in vec3 normal;
noperspective in vec2 texture_coord;

out vec4 fragment;

uniform sampler2D texture0;

const int DITHER[16] = int[16](0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5);
const int resolution_scale = 4;

void main() {
	ivec2 coords = ivec2(gl_FragCoord.xy);
	int dcoord = ((coords.x/resolution_scale)%4) + ((coords.y/resolution_scale)%4) * 4;
	float dither = float(DITHER[dcoord])/16.;
	
	float light = max(dot(normalize(normal), normalize(vec3(1.0, 0.7, 0.8))),0.0);
	fragment = texture(texture0, texture_coord);
	fragment.xyz *= (light);

	vec3 color_lower = floor(fragment.xyz * 16.0)/16.0;
	vec3 color_higher = ceil(fragment.xyz * 16.0)/16.0;
	vec3 n_diff = (fragment.xyz-color_lower)/(color_higher-color_lower);

	fragment.x = (n_diff.x < dither) ? color_lower.x : color_higher.x;
	fragment.y = (n_diff.y < dither) ? color_lower.y : color_higher.y;
	fragment.z = (n_diff.z < dither) ? color_lower.z : color_higher.z;
}
