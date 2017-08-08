#version 400 core
out vec4 fragmentColor;
in vec3 vPositionL;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(vPositionL));
	vec3 color = texture(equirectangularMap, uv).rgb;

	fragmentColor = vec4(color, 1.0);
}
