#version 400 core

in vec3 outPosW;
in vec3 outNormalW;
in vec2 outTexCoord;
in vec4 outColor;

out vec4 FragColor;

uniform vec3 cameraPos;

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetalness;
uniform sampler2D texOcclusion;

//

#define DIRECTIONAL 0
#define POINT       0
#define MAX_LIGHTS 4
uniform struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float radius;
	int type;
} gLights[MAX_LIGHTS];

uniform int  gNumLights;

const vec3 lightDir = normalize(vec3(1, -1, 1));
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

vec3 FresnelSchlick (vec3 N, vec3 V, vec3 F0) {
	return F0 + (1 - F0) * pow(1 - dot(N, V), 5);
}

void main()
{
	vec3 N = outNormalW;
	vec3 V = normalize(cameraPos - outPosW);

	vec3  albedo    = texture(texAlbedo,    outTexCoord).rgb;
	albedo = pow(albedo, vec3(2.2));
	float roughness = texture(texRoughness, outTexCoord).r;
	float metalness = texture(texMetalness, outTexCoord).r;

	vec3 reflectance = vec3(0);
	for(int i = 0; i < gNumLights; i++)
	{
		vec3 lightDirection;

		if (gLights[i].type == DIRECTIONAL)
		{
			lightDirection = -normalize(gLights[i].direction);
		}
		else if (gLights[i].type == POINT)
		{
			lightDirection = normalize(gLights[i].position - outPosW);
		}

		vec3 L = lightDirection;
		vec3 H = normalize((L + V));
		vec3 F0 = mix(vec3(0.04), albedo.rgb, metalness);

		float D = DistributionGGX(N, H, roughness);
		vec3  F = FresnelSchlick(N, V, F0);
		float G = GeometrySmith(N, V, L, roughness);

		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = (D * F * G) / (denominator + 0.0001);

		vec3 kD = vec3(1.0) - F;

		float NdotL = max(dot(N, L), 0.0);
		reflectance += (kD * albedo / PI + specular) * vec3(2.0) * NdotL;
	}


	float ao    = texture(texOcclusion, outTexCoord).r;
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + reflectance;

	// HDR Tonemapping & Gamma Correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	FragColor = vec4(color, 1.0);
	// color.xyz = pow(color.xyz, vec3(1.0/2.2));
}
