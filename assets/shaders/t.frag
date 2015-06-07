#version 130

const int GAMMA = 1 << 6;

uniform int u_flags;
uniform sampler2D u_textureSampler;

in vec2 pass_texCoord;

void main() {
    int flags = u_flags == 0 ? GAMMA : u_flags;

    vec3 gamma = vec3(1/2.2);
    vec3 linearColor = texture(u_textureSampler, pass_texCoord).rgb;
    if ((u_flags & GAMMA) > 0)
        gl_FragColor = vec4(pow(linearColor, gamma), 1);
    else
        gl_FragColor = vec4(linearColor, 1);
}
