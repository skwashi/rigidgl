#version 130

uniform mat4 u_projMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

in vec3 a_position;
in vec3 a_normal;
in vec4 a_color;

out vec4 pass_color;

void main() {
     gl_Position = u_projMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1);
     pass_color = vec4(a_color.xyz*0.5 + a_normal * 0.5, a_color.w);
}
