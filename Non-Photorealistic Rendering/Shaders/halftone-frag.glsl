#version 400
#define PI 3.1415926535897932384626433832795

in vec2 texCoord;

// - Textura de la escena
uniform sampler2D TexScene;

// - T�cnica Halftone
uniform float threshold;
uniform vec3 colorHalftone;
uniform float intensityFactor;
uniform float halftoneSize;

layout (location = 0) out vec4 FragColor;

// - Calcular el brillo percibido de un color
float colorBrightness(vec3 color)
{
	return sqrt(color.r * color.r * 0.241 + color.g * color.g * 0.691 + color.b * color.b * 0.068);
}

void main() 
{
	// - Color y tama�o de la textura
    vec4 color = texture(TexScene, texCoord);
	vec2 texSize = textureSize(TexScene, 0) / halftoneSize;

	float sinMinus = sin(texCoord.x * texSize.x - texCoord.y * texSize.y);
    float sinPlus = sin(texCoord.x * texSize.x + texCoord.y * texSize.y);

	// - Calcular brillo percibido por un color, de forma que el halftone ser� m�s claro u oscuro
	//   en funci�n de como sea el propio color
    float brightness = colorBrightness(color.rgb);
    
	// - Dibujar l�neas diagonales en ambos sentidos si se cumple la condici�n, para dibujar rombos
    if (sinMinus < threshold && sinPlus < threshold)
    {
		color += (vec4(colorHalftone, 1.0) * sin(2.0 * PI * brightness) * intensityFactor);
    }
    
    FragColor = color;
}   