#version 130

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 attenuation;
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
     vec3 lp = (vec4(light.position, 1)).xyz;
     vec3 lp1 = (vec4(light.position + vec3(5, 0, 0), 1)).xyz;
     vec3 cp = vec3(vec4(pass_position, 1));
     float radius = length(lp1 - lp);
     float dist = length(cp - lp);
     if (dist > radius)
             FragColor = vec4(1, 0, 0, 1);
     else {
             float a = attenuation(dist);
             FragColor = vec4((light.ambient + light.diffuse), 1);
     }
}
