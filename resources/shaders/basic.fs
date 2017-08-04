#version 330 core
in vec3 outColor;
in vec2 outTexCoord;

out vec4 color;

uniform sampler2D diffuse;

void main()
{
  //color = vec4(outColor, 1.0f);
  color = texture(diffuse, outTexCoord);
}
