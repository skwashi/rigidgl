#version 130

uniform sampler2D u_textureSampler;

in vec2 pass_texCoord;

void main() {
    vec4 color = texture(u_textureSampler, pass_texCoord);
    float z = color.r;
    float n = 0.1;
    float f = 200;
    float c = (2.0 * n) / (f + n - z * (f - n));
    gl_FragColor = color;
}
