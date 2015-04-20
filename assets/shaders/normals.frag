#version 130

in vec3 pass_normal;

void main() {
     gl_FragColor = vec4(pass_normal, 1);
}
