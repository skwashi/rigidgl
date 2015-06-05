#version 130

uniform samplerCube u_textureSampler;

in vec3 pass_texCoord;

void main() {
    gl_FragColor = texture(u_textureSampler, pass_texCoord);
}
