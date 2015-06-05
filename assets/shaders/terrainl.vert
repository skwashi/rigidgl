#version 130
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;
uniform mat3 u_normal;

in vec4 a_position;
in vec3 a_normal;
in vec2 a_texCoord;

out vec3 pass_position;
out vec3 pass_position_w;
out vec3 pass_normal;
out vec2 pass_texCoord;

void main() {
    float height = a_position.y;
    pass_position_w = (u_m * a_position).xyz;
    vec4 pos = u_v * u_m * a_position;
    vec3 normal_w = u_normal * a_normal;
    pass_position = pos.xyz;
    pass_normal = (u_v * vec4(normal_w, 0)).xyz;
    pass_texCoord = a_texCoord;
    gl_Position = u_p * pos;
}
