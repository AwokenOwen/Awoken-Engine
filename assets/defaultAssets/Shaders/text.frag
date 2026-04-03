#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    gl_FragColor = vec4(color, 1.0) * sampled;
}  