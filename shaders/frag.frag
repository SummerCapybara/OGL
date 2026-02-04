#version 330 core
in vec3 uv;
out vec4 FragColor;

void main() {
    FragColor = vec4(uv.x, uv.y, uv.z, 1.0);
}
