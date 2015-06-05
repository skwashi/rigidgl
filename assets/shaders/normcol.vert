#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec4 a_position;
in vec3 a_normal;
in vec4 a_color;

out vec4 pass_color;

void main() {
    gl_Position = u_p * u_v * u_m * a_position;
    pass_color = vec4(0.6, 0.6, 1, 0.6);
}
