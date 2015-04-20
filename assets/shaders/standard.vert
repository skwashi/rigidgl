#version 130

uniform mat4 u_projMatrix;
uniform mat4 u_viewMatrix;

in vec3 a_position;
in vec4 a_color;

out vec4 pass_color;

void main() {
     pass_color = a_color;
     gl_Position = u_projMatrix * u_viewMatrix * vec4(a_position, 1);
}
