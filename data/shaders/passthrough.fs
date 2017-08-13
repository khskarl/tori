#version 400 core
out vec4 fragmentColor;

in vec2 TexCoords;

uniform float exposureLevel;
uniform sampler2D screenColorTexture;

void main()
{
	vec3 hdrColor = texture(screenColorTexture, TexCoords).rgb;

	// HDR Tonemapping
	vec3 color = vec3(1.0) - exp(-hdrColor * exposureLevel);

	// Gamma correction
	const float gamma = 2.2;
	color = pow(color, vec3(1.0 / gamma));

	fragmentColor = vec4(color, 1.0f);
}
