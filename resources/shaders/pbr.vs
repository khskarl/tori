#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec4 vColor;

out vec3 outPosW;
out vec3 outNormalW;
out vec2 outTexCoord;
out vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outPosW     = vec3(model * vec4(vPosition, 1.0));
	outNormalW  = normalize(mat3(model) * vNormal);
	outTexCoord = vTexCoord;
	outColor    = vColor;
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}
