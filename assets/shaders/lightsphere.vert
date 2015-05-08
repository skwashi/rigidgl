#version 130

uniform mat4 u_p;
uniform mat4 u_v;

in vec4 a_position;

out vec3 pass_position;

void main() {
    vec4 pos = a_position;
    gl_Position = u_p * pos;
    pass_position = pos.xyz;
}
