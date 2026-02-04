#version 330 core
layout(location = 0) in vec3 aPos;
out vec3 uv;

uniform float offset;
uniform float vertOffset;

void main() {
    uv = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos.x + offset, aPos.y + vertOffset, aPos.z, 1.0);
}