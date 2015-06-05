#version 130

struct Material {
    vec3 albedo;
    float metalness;
    float specularPower;
};

uniform sampler2D u_texture;
uniform Material mat;

in vec3 pass_position;
in vec3 pass_normal;
in vec2 pass_texCoord;

out vec4 out_ambient;
out vec4 out_albedo;
out vec2 out_normal;
out float out_z;

vec2 signNotZero(vec2 v) {
    return vec2((v.x >= 0.0) ? 1.0 : -1.0, (v.y >= 0.0) ? 1.0 : -1.0);
}

vec2 encodeNormalOct32(vec3 n) {
    vec2 p = n.xy * (1.0 / (abs(n.x) + abs(n.y) + abs(n.z)));
    return (n.z <= 0.0) ? ((1.0 - abs(p.yx)) * signNotZero(p)) : p;
}

void main() {
    vec3 normal = normalize(pass_normal);
    out_ambient = vec4(0.01 * mat.ambient, 1);
    out_albedo = vec4(mat.diffuse, mat.shininess / 256);
    out_normal = encodeNormalOct32(normal);
    out_z = pass_position.z;
}
