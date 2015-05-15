#version 130

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material mat;

uniform samplerCube u_textureSampler;

uniform mat4 u_v;

in vec3 pass_neg_view_w;
in vec3 pass_normal_w;

void main() {
    vec3 N = normalize(pass_normal_w);
    vec3 I = normalize(pass_neg_view_w);
    vec3 R = reflect(I, N);
    vec4 light = texture(u_textureSampler, R);

    gl_FragColor = vec4(light.xyz, 1) * vec4(mat.specular, 1);
}
