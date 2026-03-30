#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// change to number of textures needed for object
// Make sure it's changed in fragment as well
#define NUM_TEXTURES 1
uniform sampler2D texture[NUM_TEXTURES];

uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
