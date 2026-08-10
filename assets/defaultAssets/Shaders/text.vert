#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in mat4 aTransform;
layout (location = 5) in int aLetter;

out VS_OUT{
    vec2 TexCoords;
    flat int letter;
}vs_out;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * aTransform * vec4(aPos.xy, 0.0, 1.0);
    vs_out.letter=aLetter;
    vs_out.TexCoords = aPos.xy;
    vs_out.TexCoords.y=1.0f-vs_out.TexCoords.y;
}