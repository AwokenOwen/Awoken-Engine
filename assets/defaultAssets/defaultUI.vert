#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = (model * vec4(aPos, 1.0)).xy;
    vec4 pos = projection * model * vec4(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}  