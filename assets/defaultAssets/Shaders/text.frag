#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec2 TexCoords;
    flat int letter;
}fs_in;


uniform sampler2DArray text;
uniform vec3 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vec3(fs_in.TexCoords.xy, fs_in.letter)).r);
    FragColor = vec4(color, 1.0) * sampled;
}