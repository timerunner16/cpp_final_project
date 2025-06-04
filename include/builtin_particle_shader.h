#include <string>

const std::string particle_shader_source = R"V0G0N(
#ifdef VERTEX_SHADER
layout (location=0) in vec2 i_vertex;

flat out int instanceID;

uniform vec3 offsets[256];

uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 nview = view;
	nview[0] = vec4(1,0,0,0);
	nview[1] = vec4(0,1,0,0);
	nview[2] = vec4(0,0,1,0);
	
	gl_Position = projection * (nview * vec4(i_vertex, 0.0, 1.0) + view * vec4(offsets[gl_InstanceID], 1.0));
	instanceID = gl_InstanceID;
}
#endif
#ifdef FRAGMENT_SHADER
out vec4 fragment;

flat in int instanceID;

uniform vec4 color;
uniform float transparencies[256];

const int DITHER[16] = int[16](0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5);

void main() {
	ivec2 coords = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	int dcoord = (coords.x%4) + (coords.y%4) * 4;
	float dither = float(DITHER[dcoord])/16.;
	
	if (dither > transparencies[instanceID]) discard;
	
	fragment = color;
}
#endif
)V0G0N";
