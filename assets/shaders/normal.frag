#version 130

uniform sampler2D normalMap;

in vec2 pass_texCoord;

vec3 decodeNormal(vec2 enc) {
    vec4 nn = vec4(enc * 2, 0, 0) + vec4(-1, -1, 1, -1);
    float l = dot(nn.xyz, -nn.xyw);
    nn.z = l;
    nn.xy *= sqrt(l);
    return nn.xyz * 2 + vec3(0, 0, -1);
}

vec3 decodeNormalLA(vec2 enc) {
    float d = dot(enc, enc);
    float f = sqrt(1 - d/4);
    return vec3(enc.x * f, enc.y * f, -1 + d/2);
}

vec2 signNotZero(vec2 v) {
    return vec2((v.x >= 0.0) ? 1.0 : -1.0, (v.y >= 0.0) ? 1.0 : -1.0);
}

vec3 decodeNormalOct32(vec2 enc) {
    vec3 v = vec3(enc.xy, 1.0 - abs(enc.x) - abs(enc.y));
    if (v.z < 0)
        v.xy = (1.0 - abs(v.yx)) * signNotZero(v.xy);
    return normalize(v);
}

void main() {
    vec4 color = texture(normalMap, pass_texCoord);
    vec3 n = decodeNormalOct32(color.xy);
    vec3 nc = clamp(n, vec3(0, 0, 0), vec3(1, 1, 1));
    vec3 gamma = vec3(1/2.2);
    gl_FragColor = vec4(nc, 1);
}
