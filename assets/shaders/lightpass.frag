#version 130

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    float radius;
};

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
    vec2 texCoord = calcTexCoord();
    vec3 pos = calcPosition(texture(depthMap, texCoord).r, texCoord);
    vec3 normal = decodeNormalOct32(texture(normalMap, texCoord).xy);
    vec4 albedoShininess = texture(albedoMap, texCoord);
    vec3 albedo = albedoShininess.rgb;
    float specularPower = albedoShininess.a * 256;

    vec3 V = normalize(-pos);
    vec3 N = normal;
    vec3 L;
    float a;

    if (light.type == 1) {
        L = -light.direction;
        a = 1;
    } else {
        vec3 surf_to_light = light.position - pos;
        float distance = length(surf_to_light);
        if (distance > light.radius)
            discard;
        vec3 L = surf_to_light / distance;
        a = attenuation(light, distance);
    }

    vec3 linearColor = light.ambient * albedo;

    linearColor += a * light.diffuse * albedo * diffuse(N, L);

    linearColor += a * light.specular * vec3(0.04)
        * specular(N, L, V, specularPower);

    gl_FragColor = vec4(linearColor, 1);
}
