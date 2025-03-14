#ifdef VERTEX_SHADER
layout (location=0) in vec4 i_vertex;

out vec2 texture_coord;

void main() {
	gl_Position = vec4(i_vertex.xy, 0.0, 1.0);
	texture_coord = i_vertex.zw;
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 texture_coord;

out vec4 fragment;

uniform sampler2D texture0;
uniform ivec2 window_resolution;
uniform int window_downscale;

const vec3 PALETTE_SIZE = vec3(pow(2,3),pow(2,3),pow(2,2));//vec3(pow(2,5), pow(2,6), pow(2,5));

const int DITHER[16] = int[16](0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5);

void main() {
	ivec2 coords = ivec2(gl_FragCoord.xy)/window_downscale*window_downscale;
	int dcoord = ((coords.x)%4) + ((coords.y)%4) * 4;
	float dither = float(DITHER[dcoord])/16.;
	
	fragment = texture(texture0, texture_coord);
	
	vec3 color_lower = floor(fragment.rgb*PALETTE_SIZE)/PALETTE_SIZE;
	vec3 color_higher = ceil(fragment.rgb*PALETTE_SIZE)/PALETTE_SIZE;
	vec3 n_diff = (fragment.xyz-color_lower)/(color_higher-color_lower);

	fragment.x = (n_diff.x < dither) ? color_lower.x : color_higher.x;
	fragment.y = (n_diff.y < dither) ? color_lower.y : color_higher.y;
	fragment.z = (n_diff.z < dither) ? color_lower.z : color_higher.z;
}
#endif
