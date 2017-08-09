#version 400 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBinormal;
layout (location = 5) in vec4 aColor;

out vec3 vPosW;
out vec3 vNormalW;
out vec2 vTexCoord;
out vec4 vColor;
out mat3 vTBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vPosW     = vec3(model * vec4(aPosition, 1.0));
	vNormalW  = normalize(mat3(model) * aNormal);
	vTexCoord = aTexCoord;
	vColor    = aColor;
	gl_Position = projection * view * model * vec4(aPosition, 1.0);

	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBinormal, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	vTBN = (mat3(T, B, N));
}
