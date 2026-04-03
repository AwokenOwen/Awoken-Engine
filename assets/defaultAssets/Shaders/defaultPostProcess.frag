#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textures[1];

void main()
{
    vec4 base = texture2D(texture[0], TexCoords);
    // Do stuff



    FragColor = base;
}