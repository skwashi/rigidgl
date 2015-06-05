#version 130

uniform sampler2D depthMap;
//uniform float zNear;
uniform float zFar;

in vec2 pass_texCoord;

float calcNormZ(float z) {
    //z * d = (f+n)/(f-n) * z + 2fn/(f-n)
    // z * d * (f-n) = z (f+n) + 2fn
    // z (d * (f-n) - (f + n)) = 2 f n
    // z = - 2 f n / (f + n - d * ( f - n))
    // z/f = - 2 n / (f + n - d * ( f - n)) -- negative
    //return 2.0 * zNear / (zFar + zNear - depth * (zFar - zNear));
    return -z / zFar;
}

void main() {
    vec4 color = texture(depthMap, pass_texCoord);
    float depth = color.r;
    float c = pow(calcNormZ(depth), 1/2.2);
    gl_FragColor = vec4(c, c, c, 1);
}
