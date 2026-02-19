#version 330 core
layout(location = 0) in vec3 aPos;
out vec3 aColor;
uniform mat4 projection;


void main() {
    aColor = vec3(1.0, 0.0, 0.0);
    vec4 pos = vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_Position = projection * pos;
}