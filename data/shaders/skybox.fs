#version 400 core
out vec4 fragmentColor;

in vec3 vTexCoords;
// in vec4 gl_FragCoord;

uniform samplerCube skyboxTexture;

void main()
{
  // fragmentColor = vec4(gl_FragCoord.x / 1280.0, gl_FragCoord.y/800.0, 1.0f, 1.0f);
  // fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  fragmentColor = texture(skyboxTexture, vTexCoords);
}
