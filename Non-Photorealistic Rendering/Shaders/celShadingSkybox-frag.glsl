#version 400

in vec3 texCoord;

uniform samplerCube SamplerSkybox;
uniform vec3 Ia; // - Intensidad de fuente luminosa ambiente
uniform float tones; // - Nº de tonos para la técnica Toon shading o Cel-Shading

layout (location = 0) out vec4 FragColor;

void main()
{    
	// - Samplear textura
    vec4 color = texture(SamplerSkybox, texCoord);

	// - Calcular media de colores y aplicar niveles de tonos
	float average = (color.r + color.g + color.b) / 3.0;
	average = floor(average * tones) / tones;

	// - Calcular factor ambiente
	vec3 ambient = Ia * color.rgb;

	// - Aplicar iluminación ambiente para calcular el color final
	color.rgb = ambient + (average * color.rgb);

	FragColor = color;
}