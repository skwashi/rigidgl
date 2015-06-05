#version 130

uniform sampler2D u_textureSampler;

in vec2 pass_texCoord;

void main() {
    vec3 gamma = vec3(1/2.2);
    vec3 linearColor = texture(u_textureSampler, pass_texCoord).rgb;
    gl_FragColor = vec4(pow(linearColor, gamma), 1);
}
