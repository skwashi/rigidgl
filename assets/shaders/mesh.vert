#version 130
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

in vec4 a_position;
in vec3 a_normal;

out vec3 pass_normal;
out vec3 pass_position;

void main() {
    pass_normal = (u_v * u_m * vec4(a_normal, 0)).xyz;
    vec4 pos = u_v * u_m * a_position;
    pass_position = pos.xyz;
    gl_Position = u_p * pos;
}
