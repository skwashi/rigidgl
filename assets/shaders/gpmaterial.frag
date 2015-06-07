#version 130

struct Material {
    vec3 albedo;
    vec3 emission;
    float metallic;
    float roughness;
};

uniform sampler2D u_texture;
uniform Material mat;

in vec3 pass_position;
in vec3 pass_normal;
in vec2 pass_texCoord;

out vec4 out_emission;
out vec4 out_albedo;
out vec4 out_normal;
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
    out_emission = vec4(mat.emission, 0.5);
    out_albedo = vec4(mat.albedo, mat.roughness);
    out_normal = vec4(encodeNormalOct32(normal), mat.metallic, 0);
    out_z = pass_position.z;
}
