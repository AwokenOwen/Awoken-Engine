#version 330 core

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{
	gl_FragColor = texture2D(tex, TexCoords);
	if (gl_FragColor.a < 0.1)
	{
		discard;
	}
}