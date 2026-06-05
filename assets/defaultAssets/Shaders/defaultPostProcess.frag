#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture[1];

void main()
{
    vec4 base = texture2D(texture[0], TexCoords);
    // Do stuff



    FragColor = base;
}