#version 400 core

#define DIRECTIONAL 0
#define POINT       1
#define MAX_LIGHTS 4
uniform struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;
	int type;
} gLights[MAX_LIGHTS];

in vec3 vPosW;
in vec3 vNormalW;
in vec2 vTexCoord;
in vec4 vColor;
in mat3 vTBN;

out vec4 fragmentColor;

uniform vec3 cameraPos;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetalness;
uniform sampler2D texOcclusion;
uniform samplerCube irradianceMap;

uniform int gNumLights;

const float PI = 3.14159265359;

float DistributionGGX (vec3 N, vec3 H, float roughness) {
	float alpha_2 = roughness * roughness;
	float NdotH_2 = pow(max(dot(N, H), 0.0), 2);
	float denominator = PI * pow(NdotH_2 * (alpha_2 - 1.0) + 1.0, 2);
	return alpha_2 / denominator;
}

float GeometrySchlickGGX (float NdotV, float k) {
	float numerator   = NdotV;
	float denominator = NdotV * (1 - k) + k + 0.0001;
	return numerator / denominator;
}

float GeometrySmith (vec3 N, vec3 V, vec3 L, float roughness) {
	float k = pow(roughness + 1, 2) / 8.0;
	float NdotV = max(dot(N, V), 0);
	float NdotL = max(dot(N, L), 0);
	return GeometrySchlickGGX(NdotV, k) *
	       GeometrySchlickGGX(NdotL, k);
}

vec3 FresnelSchlick (float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlickRoughness (float cosTheta, vec3 F0, float roughness) {
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{

	vec3  albedo    = pow(texture(texAlbedo,    vTexCoord).rgb, vec3(2.2));
	vec3  normal    = texture(texNormal,    vTexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = mix(vec3(0, 0, 1), normal, 1.0);
	normal = normalize(vTBN * normal);
	float roughness = texture(texRoughness, vTexCoord).r;
	float metalness = texture(texMetalness, vTexCoord).r;
	float ao        = texture(texOcclusion, vTexCoord).r;

	vec3 F0 = mix(vec3(0.04), albedo.rgb, metalness);
	vec3 N = normal;
	vec3 V = normalize(cameraPos - vPosW);

	float NdotV = max(dot(N, V), 0.0f);

	vec3 reflectance = vec3(0);
	for(int i = 0; i < gNumLights; i++)
	{
		vec3 lightDirection;
		float attenuation = 1.0;
		if (gLights[i].type == DIRECTIONAL) {
			lightDirection = -normalize(gLights[i].direction);
		}
		else if (gLights[i].type == POINT) {
			lightDirection = normalize(gLights[i].position - vPosW);
			float d = length(gLights[i].position - vPosW);
			attenuation = 1.0 / (d * d);
		}
		vec3 radiance = gLights[i].color * attenuation;

		vec3 L = lightDirection;
		vec3 H = normalize((L + V));
		float HdotV = max(dot(H, V), 0.0f);
		float NdotL = max(dot(N, L), 0.0);

		float D = DistributionGGX(N, H, roughness);
		vec3  F = FresnelSchlick(HdotV, F0);
		float G = GeometrySmith(N, V, L, roughness);

		float denominator = 4 * NdotV * NdotL;
		vec3 specular = (D * F * G) / (denominator + 0.001);

		vec3 kD = vec3(1.0) - F;
		kD *= 1.0 - metalness;

		reflectance += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 kS = FresnelSchlickRoughness(NdotV, F0, roughness);
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(irradianceMap, N).rgb * 1.0f;
	vec3 ambient = (kD * irradiance * albedo) * ao;
	vec3 color = ambient + reflectance;

	fragmentColor = vec4(color, 1.0);
}
