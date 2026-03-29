#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

uniform mat4 model;

void main()
{
    TexCoords = (vec2(aPos.x + 1.0f, aPos.y - 1.0f) * 0.5);
    vec4 pos = model * vec4(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}  