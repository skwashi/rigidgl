#version 130

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    float radius;
};

uniform mat4 u_p;
uniform mat4 u_v;

uniform Light light;

in vec3 a_position;

out vec3 pass_position;

void main() {
    vec4 pos = vec4(light.radius * a_position + light.position, 1);
    gl_Position = u_p * pos;
    pass_position = pos.xyz;
}
