#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec3 a_position;
in vec3 a_normal;
in vec4 a_color;

out vec4 pass_color;

void main() {
     gl_Position = u_p * u_v * u_m * vec4(a_position, 1);
     pass_color = vec4(a_position + 0.5, 1);//vec4(a_color.xyz*0.5 + a_normal * 0.5, a_color.w);
}
