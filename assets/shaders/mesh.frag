// Fragment shader
#version 130

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

uniform Light light;

in vec3 pass_normal;
in vec3 pass_position;

out vec4 FragColor;

float diffuse(vec3 N, vec3 L) {
    return max(dot(N, L), 0);
}

float specular(vec3 N, vec3 L, vec3 V, float power) {
    vec3 H = normalize(L + V);
    float nF = (power + 8) / 8;
    return nF * pow(max(dot(H, N), 0), power);
}

float attenuation(float distance) {
    float a = light.attenuation.x;
    float b = light.attenuation.y;
    float c = light.attenuation.z;
    return 1 / (a + b * distance + c * distance * distance);
}

void main() {
    vec3 surf_to_light = light.position - pass_position;
    float distance = length(surf_to_light);
    vec3 L = surf_to_light / distance;
    vec3 V = normalize(-pass_position);
    vec3 N = normalize(pass_normal);

    float a = attenuation(distance);
    float secularPower = 1;

    vec3 diffusePart = a * light.diffuse * diffuse(N, L);
    vec3 specularPart = a * light.specular * specular(N, L, V, 32);

    vec3 gamma = vec3(1.0/2.2);
    vec3 linearColor = light.ambient + diffusePart + specularPart;
    FragColor = vec4(pow(linearColor, gamma), 1);
}
