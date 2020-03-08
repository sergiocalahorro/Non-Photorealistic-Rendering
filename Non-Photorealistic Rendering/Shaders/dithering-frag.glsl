#version 400

in vec2 texCoord;

// - Textura de la escena
uniform sampler2D TexScene;

// - T�cnica Dithering

// - Umbrales en los que se dibujar�n las l�neas
uniform float threshold1;
uniform float threshold2;
uniform float threshold3;
uniform float threshold4;

// - Factores para modificar el color de la textura para dibujar las l�neas
uniform float intensityThreshold1;
uniform float intensityThreshold2;
uniform float intensityThreshold3;
uniform float intensityThreshold4;

// - Densidad de las l�neas a dibujar (c�mo de juntas estar�n dibujadas)
uniform float density;

// - Anchura de las l�neas a dibujar
uniform float width;

// - Dibujar usando colores de la textura o en blanco y negro
uniform bool useSceneColor;

layout (location = 0) out vec4 FragColor;

// - Calcular el brillo percibido de un color
float colorBrightness(vec3 color)
{
	// - Luminancia (color percibido)
	vec3 luminance = vec3(0.2326, 0.7152, 0.0722);
	return sqrt(color.r * color.r * luminance.r + color.g * color.g * luminance.g + color.b * color.b * luminance.b);
}

vec3 dithering(vec3 texDiffuse)
{
	// - Coordenadas del pixel
	vec2 pixel = gl_FragCoord.xy;

	// - Color de la textura
	vec3 color = texDiffuse;

	// - RGB a 'luma' (informaci�n de la luminosidad de la imagen)
    float brightness = colorBrightness(color);

	// - Obtener los colores mayor y menor de los canales RGB de la textura
	float minChannelColor = min(min(color.r, color.g), color.b);
	float maxChannelColor = max(max(color.r, color.g), color.b);
	float deltaChannelColor = maxChannelColor - minChannelColor;

	// - Factor para comparar la variaci�n en los valores m�nimo y m�ximo de los canales de la textura
	float deltaFactor = 0.1f;

	// - Color base
	if (useSceneColor)
	{
		// - Colorear usando el color blanco y el de la textura
		if (deltaChannelColor > deltaFactor)
		{
			color.rgb = color.rgb / maxChannelColor;
		}
		else
		{
			color.rgb = vec3(1.0);
		}
	}
	else if (!useSceneColor)
	{
		// - Colorear en blanco y negro
		if (deltaChannelColor > deltaFactor)
		{
			color.rgb = mix(vec3(0.0), vec3(maxChannelColor), 1.0);
		}
		else
		{
			color.rgb = vec3(1.0);
		}
	}

	// - Color final del fragmento
	vec3 finalColor = vec3(1.0);

	// - Comparar luminosidad con los distintos umbrales y, en funci�n de los resultados, dibujar las l�neas
	//   en distintas direcciones

	// - L�neas que representan el color de relleno

	// - Dibujar l�neas diagonales hacia la derecha
    if (brightness < threshold1) 
    {
		if (mod(pixel.x + pixel.y, density) >= width)
		{
			finalColor = vec3(color.rgb * intensityThreshold1);
		}
    }
  
  	// - Dibujar l�neas diagonales hacia la izquierda
    if (brightness < threshold2) 
    {
		if (mod(pixel.x - pixel.y, density) >= width)
		{
			finalColor = vec3(color.rgb * intensityThreshold2);
		}
    }

	// - L�neas que representan las zonas con sombra
  
  	// - Dibujar l�neas diagonales hacia la derecha
    if (brightness < threshold3) 
    {
		if (mod(pixel.x + pixel.y - (density * 0.5), density) >= width)
		{
			finalColor = vec3(color.rgb * intensityThreshold3);
		}
    }
  
	// - Dibujar l�neas diagonales hacia la izquierda
    if (brightness < threshold4) 
    {
		if (mod(pixel.x - pixel.y - (density * 0.5), density) >= width)
		{
			finalColor = vec3(color.rgb * intensityThreshold4);
		}
    }

	return finalColor;
}

void main()
{
	vec4 texDiffuse = texture(TexScene, texCoord);

	vec3 finalColor = dithering(texDiffuse.rgb);	

	FragColor = vec4(finalColor, 1.0);
}