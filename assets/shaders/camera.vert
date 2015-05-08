#version 130

uniform mat4 u_p;
uniform mat4 u_v;

in vec4 a_position;

out vec3 pass_position;

void main() {
        gl_Position = a_position * vec4(0.5, 0.5, 0.5, 1);
        pass_position = (inverse(u_p) * gl_Position).xyz;
}
