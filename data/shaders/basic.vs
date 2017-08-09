#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vColor;

out vec3 outColor;
out vec2 outTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	outColor.z = 0.0f;
	outColor.xy = vTexCoord;
	outTexCoord = vTexCoord;
}
