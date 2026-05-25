#version 460 core
layout(location = 0) in vec2 vertPos;

out vec3 texCoords;
uniform mat4 camera;
uniform vec2 chunkPos;
uniform float chunkSlice;

void main() {

    texCoords = vec3(vertPos * 0.5 + 0.5, chunkSlice);
    gl_Position = camera * vec4(vertPos + chunkPos, 0.0, 1.0);
}
