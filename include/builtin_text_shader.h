#include <string>

const std::string shader_source = "#ifdef VERTEX_SHADER\n\
layout (location=0) in vec4 i_vertex;\n\
out vec2 texture_coord;\n\
void main() {\n\
	gl_Position = vec4(i_vertex.xy, 0.0, 1.0);\n\
	texture_coord = i_vertex.zw;\n\
}\n\
#endif\n\
#ifdef FRAGMENT_SHADER\n\
in vec2 texture_coord;\n\
out vec4 fragment;\n\
uniform sampler2D texture0;\n\
uniform vec4 modulate;\n\
void main() {\n\
	fragment = texture(texture0, texture_coord)*modulate;\n\
}\n\
#endif";
