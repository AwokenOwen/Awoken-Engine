#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    vec4 pos = model * vec4(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}  