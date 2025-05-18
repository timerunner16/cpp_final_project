#include <string>

const std::string particle_shader_source = R"V0G0N(
#ifdef VERTEX_SHADER
layout (location=0) in vec2 i_vertex;

uniform vec3 offsets[256];

uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 nview = view;
	nview[0] = vec4(1,0,0,0);
	nview[1] = vec4(0,1,0,0);
	nview[2] = vec4(0,0,1,0);
	
	gl_Position = projection * (nview * vec4(i_vertex, 0.0, 1.0) + view * vec4(offsets[gl_InstanceID], 1.0));
}
#endif
#ifdef FRAGMENT_SHADER
out vec4 fragment;

uniform vec4 color;

void main() {
	fragment = color;
}
#endif
)V0G0N";
