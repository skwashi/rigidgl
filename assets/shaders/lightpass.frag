#version 130

const int ALL = ~0;
const int AMBIENT = 1 << 2;
const int DIFFUSE = 1 << 3;
const int SPECULAR = 1 << 4;
const int ATTENUATION = 1 << 5;
const int GAMMA =  1 << 6;
const int STANDARD = AMBIENT | DIFFUSE | SPECULAR | ATTENUATION | GAMMA;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 intensity;
    vec3 attenuation;
    float radius;
};

uniform int u_flags;
uniform Light light;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform ivec2 u_screenSize;
uniform float xRatio;
uniform float yRatio;

in vec3 pass_direction;

vec2 calcTexCoord() {
    return gl_FragCoord.xy / u_screenSize;
}

vec3 calcPosition(float z, vec2 coord) {
    return pass_direction * z;
}

vec2 signNotZero(vec2 v) {
    return vec2((v.x >= 0.0) ? 1.0 : -1.0, (v.y >= 0.0) ? 1.0 : -1.0);
}

vec3 decodeNormalOct32(vec2 enc) {
    vec3 v = vec3(enc.xy, 1.0 - abs(enc.x) - abs(enc.y));
    if (v.z < 0)
        v.xy = (1.0 - abs(v.yx)) * signNotZero(v.xy);
    return normalize(v);
}

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

void main() {
    int flags;
    if (u_flags == 0)
        flags = STANDARD;
    else
        flags = u_flags;

    vec2 texCoord = calcTexCoord();
    vec3 pos = calcPosition(texture(depthMap, texCoord).r, texCoord);
    vec4 albedoRoughness = texture(albedoMap, texCoord);
    vec3 normalMetallic = texture(normalMap, texCoord).rgb;
    vec3 normal = decodeNormalOct32(normalMetallic.rg);
    vec3 albedo = albedoRoughness.rgb;
    float roughness = albedoRoughness.a;
    float metallic = normalMetallic.b;

    vec3 V = normalize(-pos);
    vec3 N = normal;
    vec3 L;
    float a = 1;
    float d = 0;

    if (light.type == 1) {
        L = -light.direction;
    } else {
        vec3 surf_to_light = light.position - pos;
        d = length(surf_to_light);
        L = surf_to_light / d;
        if ((flags & ATTENUATION) > 0)
            a = attenuation(light, d);
    }

    if (d > light.radius)
        discard;
    else {
        float diffuseI = 0, specularI = 0;

        if ((flags & AMBIENT) > 0)
            diffuseI += light.intensity.x;
        if ((flags & DIFFUSE) > 0)
            diffuseI += diffuse(N, L) * light.intensity.y;
        if ((flags & SPECULAR) > 0) {
            float specularPower = exp2(10*(1-roughness) + 1);
            specularI = specular(N, L, V, specularPower)
                * light.intensity.z;
        }

        float intensity = a *
            ((1 - metallic) * diffuseI + metallic * specularI);

        vec3 linearColor = albedo * light.color * intensity;

        gl_FragColor = vec4(linearColor, 1);
    }
}
