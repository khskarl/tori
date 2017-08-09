#version 400 core
layout (location = 0) in vec3 aPosition;

out vec3 vPositionL;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vPositionL = aPosition;
	gl_Position =  (projection * view * vec4(vPositionL, 1.0)).xyww;
}
