#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec3 a_position;
in vec2 a_texCoord;
in vec4 a_color;

out vec2 pass_texCoord;
out vec4 pass_color;

void main() {
     gl_Position = u_p * u_v * vec4(a_position, 1);
     pass_texCoord = a_texCoord;
     pass_color = a_color;
}
