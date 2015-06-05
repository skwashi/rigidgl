#version 130

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;
uniform mat3 u_normal;

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texCoord;

out vec3 pass_position;
out vec3 pass_position_w;
out vec3 pass_normal;
out vec2 pass_texCoord;

void main() {
    vec4 pos_w = u_m * vec4(a_position, 1);
    vec4 pos_v = u_v * pos_w;
    pass_position = pos_v.xyz;
    pass_position_w = pos_w.xyz;
    pass_normal = (u_v * vec4(u_normal * a_normal, 0)).xyz;
    pass_texCoord = a_texCoord;
    gl_Position = u_p * pos_v;
}
