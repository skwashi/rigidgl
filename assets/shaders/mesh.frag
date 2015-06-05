// Fragment shader
#version 130

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

const int ALL = ~0;
const int AMBIENT = 1 << 2;
const int DIFFUSE = 1 << 3;
const int SPECULAR = 1 << 4;
const int ATTENUATION = 1 << 5;
const int GAMMA =  1 << 6;
const int INVERTCOLOR = 1 << 7;
const int NORMALCOLORS = 1 << 8;
const int STANDARD = AMBIENT | DIFFUSE | SPECULAR | ATTENUATION | GAMMA;

uniform Light light;
uniform Material mat;

uniform int u_flags;

in vec3 pass_position;
in vec3 pass_normal;
in vec3 pass_a_normal;

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

    int flags;
    if (u_flags == 0)
        flags = STANDARD;
    else
        flags = u_flags;

    vec3 surf_to_light = light.position - pass_position;
    float distance = length(surf_to_light);
    vec3 L = surf_to_light / distance;
    vec3 V = normalize(-pass_position);
    vec3 N = normalize(pass_normal);

    float a = ((flags & ATTENUATION) > 0) ? attenuation(distance) : 1;

    vec3 linearColor = vec3(0, 0, 0);
    if ((flags & AMBIENT) > 0) {
        linearColor += light.ambient * mat.ambient;
    }

    if ((flags & DIFFUSE) > 0) {
        linearColor += a * light.diffuse * mat.diffuse
            * diffuse(N, L);
    }

    if ((flags & SPECULAR) > 0) {
        linearColor += a * light.specular * mat.specular
            * specular(N, L, V, mat.shininess);
    }

    if ((flags & NORMALCOLORS) > 0) {
        linearColor = 0.5 * normalize(pass_a_normal) + 0.5;
    }

    if ((flags & INVERTCOLOR) > 0) {
        linearColor = 1 - linearColor;
    }

    vec3 out_color = linearColor;

    if ((flags & GAMMA) > 0) {
        vec3 gamma = vec3(1.0/2.2);
        out_color = pow(linearColor, gamma);
    }

    FragColor = vec4(out_color, 1);
}
