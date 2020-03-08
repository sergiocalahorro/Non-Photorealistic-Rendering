#version 400

in vec2 texCoord;

uniform sampler2D TexScene;

// - Técnica Sobel Edge Detection
uniform bool enableSobel;
uniform float threshold;
uniform vec3 edgeColor;

// - Técnica Charcoal
uniform vec3 charcoalColor;
uniform float noiseAmount;

layout (location = 0) out vec4 FragColor;

// - Sobel edge detection
float sobelEdge(vec2 offsetX, vec2 offsetY)
{
	vec4 horizontal = vec4(0.0);
	horizontal += texture(TexScene, texCoord - offsetY) * -2.0;
	horizontal += texture(TexScene, texCoord + offsetY) * 2.0;
	horizontal += texture(TexScene, texCoord - offsetX - offsetY) * -1.0;
	horizontal += texture(TexScene, texCoord - offsetX + offsetY) * 1.0;
	horizontal += texture(TexScene, texCoord + offsetX - offsetY) * -1.0;
	horizontal += texture(TexScene, texCoord + offsetX + offsetY) * 1.0;

	vec4 vertical = vec4(0.0);
	vertical += texture(TexScene, texCoord - offsetX) * 2.0;
	vertical += texture(TexScene, texCoord + offsetX) * -2.0;
	vertical += texture(TexScene, texCoord - offsetX - offsetY) * 1.0;
	vertical += texture(TexScene, texCoord - offsetX + offsetY) * -1.0;
	vertical += texture(TexScene, texCoord + offsetX - offsetY) * 1.0;
	vertical += texture(TexScene, texCoord + offsetX + offsetY) * -1.0;

	return length(sqrt(horizontal.rgb * horizontal.rgb + vertical.rgb * vertical.rgb));
}

// - Ruido aleatorio
float randomNoise2D(vec2 uv)
{
	return fract(sin(dot(uv, vec2(12.9898, 78.233)) * 43758.5453));
}

void main()
{
	// - Luminancia (color percibido) o brillo
	vec4 luma = vec4(0.2326, 0.7152, 0.0722, 0.0);

	vec4 texColor = texture(TexScene, texCoord);
	vec2 resolution = textureSize(TexScene, 0).xy;

	vec2 offsetX = vec2(1.0 / resolution.x, 0.0);
	vec2 offsetY = vec2(0.0, 1.0 / resolution.y);

	// - Aplicar algoritmo Sobel para detectar y dibujar los contornos
	float edge = sobelEdge(offsetX, offsetY);

	// - Color del fragmento
	vec4 color;

	// - Dibujar contornos/relleno
	if (enableSobel && (edge > (threshold * 8.0)))
	{
		color.rgb = edgeColor;
	}
	else
	{
		color.a = threshold;
		color.rgb = charcoalColor;
	}

	// - Aplicar luminancia
	color += dot(texColor, luma);

	// - Generar factor de ruido aleatorio y aplicarlo al color 
	float noise = (randomNoise2D(texCoord) - 0.5) * noiseAmount;
	color.rgb += vec3(noise);

	FragColor = vec4(color.rgb, 1.0);
}