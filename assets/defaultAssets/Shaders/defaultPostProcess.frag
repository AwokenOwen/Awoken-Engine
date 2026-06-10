#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D post;

void main()
{
    vec4 base = texture(post, TexCoords);
    // Do stuff



    FragColor = base;
}