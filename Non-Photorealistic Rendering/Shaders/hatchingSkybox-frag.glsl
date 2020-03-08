#version 400

in vec3 texCoord;

uniform samplerCube SamplerSkybox;

// - Técnica Hatching
uniform sampler2D hatchBright;
uniform sampler2D hatchDark;
uniform float density;
uniform mat3 rotationMatrix;

out vec4 FragColor;

// - Hatching
vec3 hatching(vec3 uv, float intensity)
{
	// - Samplear texturas de hatching
	vec3 hatchingDark = texture2D(hatchDark, uv.xy * density).rgb;
    vec3 hatchingBright = texture2D(hatchBright, uv.xy * density).rgb;

	// - Brillo del fragmento
    vec3 brightness = vec3(max(0.0, intensity - 1.0));

	// - Factor que se aplica a cada color de las texturas de hatching
    vec3 strengthDark = clamp(vec3(0.0, -1.0, -2.0) + (intensity * 6.0), 0.0, 1.0);
    vec3 strengthBright = clamp(vec3(-3.0, -4.0, -5.0) + (intensity * 6.0), 0.0, 1.0);

	// - Corregir factores para que haya menos valores distintos de 0
    strengthDark.xy -= strengthDark.yz;
    strengthDark.z -= strengthBright.x;
    strengthBright.xy -= strengthBright.zy;

	// - Aplicar factor de intensidad a cada textura de hatching
    hatchingDark *= strengthDark;
    hatchingBright *= strengthBright;

    vec3 hatching = brightness + hatchingDark.r + hatchingDark.g + hatchingDark.b + 
								 hatchingBright.r + hatchingBright.g + hatchingBright.b;

    return hatching;
}

void main()
{    
	// - Samplear textura
    vec4 color = texture(SamplerSkybox, texCoord);

	// - Luminancia (color percibido)
	vec3 luminance = vec3(0.2326, 0.7152, 0.0722);

	// - Rotación
	vec3 newTexCoord = texCoord - vec3(0.5);
	newTexCoord *= rotationMatrix;
	newTexCoord += vec3(0.5);

	// - Hatching

	// - Obtener intensidad de color del fragmento
	float intensity = dot(color.rgb, luminance);
	color.rgb = hatching(newTexCoord * density, intensity);

	FragColor = vec4(color.rgb, 1.0);
}