#version 130

uniform sampler2D u_textureSampler;


in vec2 pass_texCoord;

void main() {
     gl_FragColor = texture(u_textureSampler, pass_texCoord);
}
