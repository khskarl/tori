#version 400 core
layout (location = 0) in vec3 aPosition;

out vec3 vTexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	vTexCoords = aPosition;

	vec4 position = projection * view * vec4(aPosition, 1.0);
	gl_Position = position.xyww;
}
