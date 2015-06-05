#version 130

in vec3 a_position;
in vec2 a_texCoord;

uniform float xRatio;
uniform float yRatio;

out vec2 pass_texCoord;
out vec3 pass_direction;

void main() {
    gl_Position = vec4(a_position, 1);
    pass_texCoord = a_texCoord;
    float x = 2 * a_texCoord.x - 1;
    float y = 2 * a_texCoord.y - 1;
    pass_direction = vec3(-xRatio * x, -yRatio * y, 1);
}
