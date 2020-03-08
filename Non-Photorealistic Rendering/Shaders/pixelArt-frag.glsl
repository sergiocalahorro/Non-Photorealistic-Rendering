#version 400

in vec2 texCoord;

// - Textura de la escena
uniform sampler2D TexScene;

// - Número de pixels en horizontal y vertical
uniform float numHorizontalPixels;
uniform float numVerticalPixels;

layout (location = 0) out vec4 FragColor;

void main()
{
	vec2 newTexCoord;

	float xPixelSize = 1.0 / numHorizontalPixels;
	float yPixelSize = 1.0 / numVerticalPixels;

	newTexCoord.x = floor(texCoord.x / xPixelSize) * xPixelSize;
	newTexCoord.y = floor(texCoord.y / yPixelSize) * yPixelSize;

	vec3 color = texture2D(TexScene, newTexCoord).rgb;
    FragColor = vec4(color, 1.0);
} 