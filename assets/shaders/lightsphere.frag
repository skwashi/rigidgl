#version 130

struct Light {
    vec3 position;
    vec3 color;
    vec3 intensity;
    vec3 attenuation;
    float radius;
};

uniform Light light;

in vec3 pass_position;

out vec4 FragColor;

float attenuation(float distance) {
    float a = light.attenuation.x;
    float b = light.attenuation.y;
    float c = light.attenuation.z;
    return 1 / (a + b * distance + c * distance * distance);
}

void main() {
    float d = length(light.position - pass_position);
    if (d > light.radius)
        discard;
    else {
        float a = attenuation(d);
        FragColor = vec4(a * light.color, a * a);
    }
}
