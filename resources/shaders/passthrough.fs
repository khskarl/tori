#version 400 core
out vec4 fragmentColor;

in vec2 TexCoords;

uniform sampler2D screenColorTexture;

void main()
{
	fragmentColor = texture(screenColorTexture, TexCoords);
}
