#version 130

uniform mat4 u_projMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

in vec3 a_position;
in vec3 a_normal;

out vec3 pass_normal;

void main() {
     gl_Position = u_projMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1);
     pass_normal = a_normal;
}
