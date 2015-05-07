#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec3 a_position;
in vec3 a_normal;

out vec3 pass_normal;

void main() {
     gl_Position = u_p * u_v * u_m * vec4(a_position, 1);
     pass_normal = a_normal;
}
