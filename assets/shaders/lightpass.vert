#version 130

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 intensity;
    vec3 attenuation;
    float radius;
};

uniform float xRatio;
uniform float yRatio;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform Light light;

in vec3 a_position;

out vec3 pass_direction;

void main() {
    if (light.type == 0) {
        vec4 pos = vec4(sqrt(2) * light.radius * a_position + light.position, 1);
        gl_Position = u_p * pos;//u_v * u_m * a_position;
    }
    else
        gl_Position = vec4(a_position, 1);
    float x_ndc = gl_Position.x / gl_Position.w;
    float y_ndc = gl_Position.y / gl_Position.w;
    pass_direction = vec3(-xRatio * x_ndc, -yRatio * y_ndc, 1);
}
