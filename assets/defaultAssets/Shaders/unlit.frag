#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
