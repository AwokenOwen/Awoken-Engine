#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 FragColor;

in vec2 TexCoords;

#define NUM_TEXTURES 1
uniform sampler2D texture[NUM_TEXTURES];

void main()
{    
	//FragColor = vec4(1.0);
	FragColor = texture2D(texture[0], normalize(TexCoords));
}