#version 130

uniform mat4 u_p;
uniform mat4 u_v;
uniform mat4 u_m;

in vec3 a_position;
in vec3 a_normal;

out vec3 pass_neg_view_w;
out vec3 pass_normal_w;

void main() {
    vec4 pos_w = u_m * vec4(a_position, 1);
    vec4 normal_w = u_m * vec4(a_normal, 0);
    vec4 cam_w = inverse(u_v) * vec4(0, 0, 0, 1);
    pass_normal_w = normal_w.xyz;
    pass_neg_view_w = (pos_w - cam_w).xyz;
    gl_Position = u_p * u_v * pos_w;
}
