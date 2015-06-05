#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;
uniform mat3 u_normal;

in vec4 a_position;
in vec3 a_normal;
in vec2 a_texCoord;

out vec3 pass_position_w;
out vec2 pass_texCoord;

const vec3 green = vec3(0, 1, 0);
const vec3 white = vec3(1, 1, 1);

void main() {
    float height = a_position.y;
    vec3 normal_w = normalize(u_normal * a_normal);
    vec3 hcol = green * (1 - height) + white * height;
    //pass_color = vec4(0.5 * normal_w + 0.5, 1);// * hcol, 1);
    pass_texCoord = a_texCoord;
    pass_position_w = (u_m * a_position).xyz;
    gl_Position = u_p * u_v * u_m * a_position;
}
