#version 130

uniform sampler2D u_textureSampler;

in vec2 pass_texCoord;
in vec4 pass_color;

void main() {
     float a = texture(u_textureSampler, pass_texCoord).r;
     gl_FragColor = vec4(pass_color.rgb, pass_color.a * a);
}
