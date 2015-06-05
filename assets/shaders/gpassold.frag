#version 130

uniform sampler2D u_sampler;

in vec3 pass_position;
in vec3 pass_normal;
in vec2 pass_texCoord;

//out vec3 out_position;
out float out_x;
out float out_y;
out float out_z;
//out vec2 out_normal;
out float out_depth;
//out vec3 out_diffuse;

vec2 encodeNormal(vec3 n) {
    vec2 enc = normalize(n.xy) * sqrt(-n.z * 0.5 + 0.5);
    return enc * 0.5 + 0.5;
}

vec2 encodeNormalLA(vec3 n) {
    float f = sqrt(2 / (1 - n.z));
    return vec2(n.x * f, n.y * f);
}

vec2 signNotZero(vec2 v) {
    return vec2((v.x >= 0.0) ? 1.0 : -1.0, (v.y >= 0.0) ? 1.0 : -1.0);
}

vec2 encodeNormalOct32(vec3 n) {
    vec2 p = n.xy * (1.0 / (abs(n.x) + abs(n.y) + abs(n.z)));
    return (n.z <= 0.0) ? ((1.0 - abs(p.yx)) * signNotZero(p)) : p;
}

void main() {
    vec3 normal = normalize(pass_normal);
    out_x = pass_position.x;
    out_y = pass_position.y;
    out_z = pass_position.z;
    //out_normal = encodeNormalOct32(normal);
    out_depth = pass_position.z;
    //out_diffuse = texture(u_sampler, pass_texCoord).rgb;
}
