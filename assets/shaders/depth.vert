#version 130

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

in vec3 a_position;
in vec2 a_texCoord;

out vec2 pass_texCoord;

void main() {
     gl_Position = u_p * u_v * u_m * vec4(a_position, 1);
     pass_texCoord = a_texCoord;
}
