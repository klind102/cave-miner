#version 460 core
layout(location = 0) in vec2 aPos;

out vec2 uv;
uniform mat4 camera;

void main() {

    uv = aPos * 0.5 + 0.5;
    gl_Position = camera * vec4(aPos, 0.0, 1.0);
}
