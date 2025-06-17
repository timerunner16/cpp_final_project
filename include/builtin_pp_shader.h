#include <string>

const std::string pp_shader_source = R"V0G0N(
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

void main() {
	fragment = texture(texture0, texture_coord);
}
#endif
)V0G0N";
