#version 130

uniform mat4 u_p;
uniform mat4 u_v;

in vec3 a_position;
in vec4 a_color;

out vec4 pass_color;

void main() {
    pass_color = vec4(1,1,1,1);//a_color;
    gl_Position = u_p * u_v * vec4(a_position, 1);
}
