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
uniform int width;
uniform int height;

in vec3 pass_position;
in vec3 pass_normal;
in vec2 pass_texCoord;

vec3 calcPosition(float z, float x1, float y1) {
    vec3 dir = vec3(xRatio * x1, yRatio * y1, -1);
    // pos = t * dir = (_,_,z) => t = z / dir.z; => pos = dir * z / dir.z
    return dir * z / dir.z;
}

vec3 calcPositionD(float depth, float x1, float y1) {
    depth = 2 * depth - 1;
    vec3 dir = vec3(xRatio * x1, yRatio * y1, -1);
    //float z = -M43 / (d + M33)
    // z = 2fn / (d(f-n) + -(f+n)
    float z = -2 * zFar * zNear / (zFar + zNear - depth * (zFar - zNear));
    return dir * z / dir.z;
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
    vec2 coord = gl_FragCoord.xy / vec2(width, height);
    float x = texture(xMap, coord).r;
    float y = texture(yMap, coord).r;
    float z = texture(zMap, coord).r;
    // vec2 normal = texture(normalMap, pass_texCoord).rg;
    // vec3 n = decodeNormalOct32(normal);
    vec3 pos = calcPosition(texture(depthMap, coord).r, 2 * coord.x - 1, 2 * coord.y - 1);
    //vec3 pos = calcPositionD(texture(depthMap, coord).r, 2 * coord.x - 1, 2 * coord.y - 1);
    vec3 realPos = pass_position;//vec3(x, y, z);
    vec3 realPosS = vec3(x, y, z);
    float ex = distance(pos.x, x) / zFar;
    float ey = distance(pos.y, y) / zFar;
    float ez = distance(pos.z, z) / zFar;
    float e = distance(pos, realPos) / length(realPos);
    //distance(realPos, realPosS) / zFar;
    //vec3 color = vec3(ex / abs(x), 0 * 100 * ey / abs(y), 0);
    vec3 color;
    if (e > pow(2, -13)*0)
        color = vec3(100*e, 0, 0);
    else
        color = vec3(0, 0, 0);
    //color = texture(depthMap, coord).rgb;
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
