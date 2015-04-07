#version 130

in vec3 a_position;
in vec4 a_color;

out vec4 pass_color;

void main() {
     pass_color = a_color;
     gl_Position = vec4(a_position, 1);
}
