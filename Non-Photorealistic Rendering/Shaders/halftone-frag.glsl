#version 400
#define PI 3.1415926535897932384626433832795

in vec2 texCoord;

// - Textura de la escena
uniform sampler2D TexScene;

// - Técnica Halftone
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
	// - Color y tamaño de la textura
    vec4 color = texture(TexScene, texCoord);
	vec2 texSize = textureSize(TexScene, 0) / halftoneSize;

	float sinMinus = sin(texCoord.x * texSize.x - texCoord.y * texSize.y);
    float sinPlus = sin(texCoord.x * texSize.x + texCoord.y * texSize.y);

	// - Calcular brillo percibido por un color, de forma que el halftone será más claro u oscuro
	//   en función de como sea el propio color
    float brightness = colorBrightness(color.rgb);
    
	// - Dibujar líneas diagonales en ambos sentidos si se cumple la condición, para dibujar rombos
    if (sinMinus < threshold && sinPlus < threshold)
    {
		color += (vec4(colorHalftone, 1.0) * sin(2.0 * PI * brightness) * intensityFactor);
    }
    
    FragColor = color;
}   