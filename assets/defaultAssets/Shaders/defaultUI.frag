#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

void main()
{    
	FragColor = vec4(1.0);
	//FragColor = texture2D(image, normalize(TexCoords));
}