#version 130
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

in vec4 a_position;
in vec3 a_normal;

out vec3 pass_position;
out vec3 pass_normal;
out vec3 pass_a_normal;

void main() {

    vec4 pos = u_v * u_m * a_position;
    pass_position = pos.xyz;
    pass_normal = (u_v * u_m * vec4(a_normal, 0)).xyz;
    pass_a_normal = a_normal;
    gl_Position = u_p * pos;
}
