#version 130

uniform sampler2D depthMap;
uniform float zNear;
uniform float zFar;
uniform float xRatio;
uniform float yRatio;

in vec2 pass_texCoord;

vec3 calcPosition(float z, vec2 coord) {
    float x = 2 * coord.x - 1;
    float y = 2 * coord.y - 1;
    vec3 dir = vec3(-xRatio * x, -yRatio * y, 1);
    return dir * z;
}

void main() {
    vec4 color = texture(depthMap, pass_texCoord);
    vec3 pos = calcPosition(color.r, pass_texCoord);
    gl_FragColor = vec4(pos, 1);
}
