#version 400 core
out vec4 fragmentColor;

in vec2 TexCoords;

uniform sampler2D screenColorTexture;

void main()
{
	const float gamma = 2.2;
	const float exposure = 1.0;
	vec3 hdrColor = texture(screenColorTexture, TexCoords).rgb;
	// HDR Tonemapping
	vec3 color = vec3(1.0) - exp(-hdrColor * exposure);
	// Gamma correction
	color = pow(color, vec3(1.0 / gamma));
	fragmentColor = vec4(color, 1.0f);
}
