#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec3 a_position;
in vec3 a_normal;

out vec3 pass_texCoord;

void main() {
     vec3 direction = mat3(u_v) * a_position;
     gl_Position = u_p * vec4(direction, 1);
     gl_Position = gl_Position.xyww;
     pass_texCoord = a_position;
}
