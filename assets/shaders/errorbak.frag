#version 130

uniform sampler2D xMap;
uniform sampler2D yMap;
uniform sampler2D zMap;
//uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform mat4 invProj;

uniform float zNear;
uniform float zFar;
uniform float xRatio;
uniform float yRatio;

in vec2 pass_texCoord;

vec3 calcPosition(float depth) {
    if (depth >= 1)
        return vec3(-1, -1, -1);
    float z_d = 2 * zFar * zNear / (zFar + zNear - depth * (zFar - zNear));
    float x = pass_texCoord.x * 2 - 1;
    float y = pass_texCoord.y * 2 - 1;
    vec3 dir = normalize(vec3(xRatio * x, yRatio * y, -1));
    return dir * depth * zFar;
}

vec3 calcPositionP(float depth) {
    if (depth >= 1)
        return vec3(-1, -1, -1);
    float x = pass_texCoord.x * 2 - 1;
    float y = pass_texCoord.y * 2 - 1;
    vec4 pos = invProj * vec4(x, y, depth, 1);
    return pos.xyz / pos.w;
}

vec3 decodeNormal(vec2 enc) {
    // vec4 nn = vec4(enc * 2, 0, 0) + vec4(-1, -1, 1, -1);
    // float l = dot(nn.xyz, -nn.xyw);
    // nn.z = l;
    // nn.xy *= sqrt(l);
    // return nn.xyz * 2 + vec3(0, 0, -1);
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
    float x = texture(xMap, pass_texCoord).r;
    float y = texture(yMap, pass_texCoord).r;
    float z = texture(zMap, pass_texCoord).r;
    // vec2 normal = texture(normalMap, pass_texCoord).rg;
    // vec3 n = decodeNormalOct32(normal);
    vec3 pos = calcPosition(texture(depthMap, pass_texCoord).r);
    vec3 realPos = vec3(x, y, z);
    float ex = distance(pos.x, x);
    float ey = distance(pos.y, y);
    float ez = distance(pos.z, z);
    float e = distance(pos, realPos) / zFar;
    //vec3 color = vec3(ex / abs(x), 0 * 100 * ey / abs(y), 0);
    vec3 color;
    if (e > pow(2, -4))
        color = vec3(100 * e, 0, 0);
    else
        color = vec3(0, 0, 0);
    // if (length(normal) < pow(2, -2)
    //     || length(vec3(x, y, z)) < pow(2, -2))
    //     color = vec3(0, 0, 0);
        // if (e > pow(2,-4))
    //     color = vec3(1, 0, 0);
    // else
    //     color = vec3(0, 0, 0);
    vec3 gamma = vec3(1.0/2.2);
    gl_FragColor = vec4(color, 1);//vec4(pow(color, gamma), 1);
}
