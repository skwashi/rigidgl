struct Light {
    int type;
    vec3 color;
    vec3 intensity;
    vec3 position;
    vec3 direction;
    vec3 attenuation;
    float radius;
};

float diffuse(vec3 N, vec3 L) {
    return max(dot(N, L), 0);
}

float specular(vec3 N, vec3 L, vec3 V, float power) {
    vec3 H = normalize(L + V);
    float nF = (power + 8) / 8;
    return nF * pow(max(dot(H, N), 0), power);
}

float attenuation(Light light, float distance) {
    float a = light.attenuation.x;
    float b = light.attenuation.y;
    float c = light.attenuation.z;
    return 1 / (a + b * distance + c * distance * distance);
}
