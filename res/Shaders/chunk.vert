#version 330 core

in vec4 position;
in vec2 offset;

uniform mat4 projection = mat4(1.0);

void main() {
    mat4 model = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        offset.x, offset.y, 0.0, 1.0
    );

    gl_Position = projection * model * position;
}