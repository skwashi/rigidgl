#version 130

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 attenuation;
    float radius;
};

uniform mat4 u_p;
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
    float distance = length(light.position - pass_position);
    if (distance > light.radius)
        discard;
    else {
        float a = attenuation(distance);
        a *= 1 / ( 1 + 0.5 * distance + 1 * distance * distance);
        float alpha = 1;
        FragColor = vec4(a * (light.ambient + light.diffuse), a * a);
    }
}
