#version 400

in vec3 texCoord;

uniform samplerCube SamplerSkybox;

out vec4 FragColor;

void main()
{    
	// - Samplear textura
    vec4 color = texture(SamplerSkybox, texCoord);

	FragColor = color;
}