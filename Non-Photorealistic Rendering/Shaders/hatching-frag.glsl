#version 400

in vec3 position;
in vec3 normal;
in vec2 texCoord;

// - Características del material
uniform vec3 KaMaterial;
uniform vec3 KdMaterial;
uniform vec3 KsMaterial;
uniform float shininess;

// - Parámetros de las fuentes luminosas
uniform vec3 Ia; // - Componente ambiente
uniform vec3 Id; // - Componente difusa
uniform vec3 Is; // - Componente especular
uniform vec3 lightPosition; // - Posición de la luz (fuentes puntual y spot)
uniform vec3 lightDirection; // - Dirección de la luz (fuentes direccional y spot). Se pasa ya normalizada
uniform	float cosGamma; // - Coseno del ángulo de la luz spot (en radianes)
uniform float spotExponent; // - Exponente de concentración de la luz de la fuente spot

// - Constantes para calcular la atenuación de la fuente luminosa
uniform float constant; // - Factor de atenuación constante
uniform float linear; // - Factor de atenuación lineal
uniform float quadratic; // - Factor de atenuación cuadrático

// - Técnica Hatching
uniform sampler2D hatchBright;
uniform sampler2D hatchDark;
uniform float density;
uniform mat2 rotationMatrix;

// - Subrutina para seleccionar el tipo de fuente luminosa a utilizar
subroutine vec3 LightFunction();
subroutine uniform LightFunction LightUniform;

layout (location = 0) out vec4 FragColor;

// - Calcular factor de atenuación de la fuente luminosa debido a la profundidad
float fatt()
{
	// - Calcular factor de atenuación de la fuente luminosa
	float dist = distance(lightPosition, position);
	float factor = min(1.0 / (constant + linear * dist + quadratic * pow(dist, 2)), 1.0);

	return factor;
}

// - Calcular fuente luminosa ambiente
subroutine(LightFunction)
vec3 AmbientLight()
{
	return (Ia * KaMaterial);
}

// - Calcular fuente luminosa puntual
subroutine(LightFunction)
vec3 PointLight() 
{
	// - Calcular vector normal (si la cara no mira hacia el observador, se usa la opuesta de la normal)
	vec3 n;

	if (gl_FrontFacing) 
	{
		n = normalize(normal);
	} 
	else 
	{
		n = normalize(-normal);
	}

	vec3 l = normalize(lightPosition - position);
	vec3 v = normalize(-position);
	vec3 r = reflect(-l, n);
	
	// - Color difuso
	vec3 Kd = KdMaterial;

	// - Componente difusa
	float cosine = max(dot(l, n), 0.0);
	vec3 diffuse = (Id * Kd * cosine);

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color = fatt() * (diffuse + specular);

	return color;
}

// - Calcular fuente luminosa direccional
subroutine(LightFunction)
vec3 DirectionalLight() 
{
	// - Calcular vector normal (si la cara no mira hacia el observador, se usa la opuesta de la normal)
	vec3 n;
	if (gl_FrontFacing) 
	{
		n = normalize(normal);
	} 
	else 
	{
		n = normalize(-normal);
	}

	vec3 l = -lightDirection;
	vec3 v = normalize(-position);
	vec3 r = reflect(-l, n);

	// - Color difuso
	vec3 Kd = KdMaterial;

	// - Componente difusa
	float cosine = max(dot(l, n), 0.0);
	vec3 diffuse = (Id * Kd * cosine);

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color = (diffuse + specular);

	return color;
}

// - Calcular fuente luminosa spot
subroutine(LightFunction)
vec3 SpotLight() 
{
	vec3 l = normalize(lightPosition - position);
	vec3 d = lightDirection;

	float spotFactor;

	// - Usar o no el exponente para calcular el spot factor
	if (dot(-l, d) < cosGamma) 
	{ 
		spotFactor = 0.0; 
	}
	else
	{
		spotFactor = pow(dot(-l, d), spotExponent); 
	}

	// - Calcular vector normal (si la cara no mira hacia el observador, se usa la opuesta de la normal)
	vec3 n;
	if (gl_FrontFacing) 
	{
		n = normalize(normal);
	} 
	else 
	{
		n = normalize(-normal);
	}

	vec3 v = normalize(-position);
	vec3 r = reflect(-l, n);

	// - Color difuso
	vec3 Kd = KdMaterial;

	// - Componente difusa
	float cosine = max(dot(l, n), 0.0);
	vec3 diffuse = (Id * Kd * cosine);

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color = spotFactor * fatt() * (diffuse + specular);

	return color;
}

// - Hatching
vec3 hatching(vec2 uv, float intensity)
{
	// - Samplear texturas de hatching
	vec3 hatchingDark = texture2D(hatchDark, uv * density).rgb;
    vec3 hatchingBright = texture2D(hatchBright, uv * density).rgb;

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
	// - Color del fragmento, aplicada la iluminación
	vec3 color = LightUniform();

	// - Luminancia (factor para calcular el color percibido en escala de grises)
	vec3 luminance = vec3(0.2326, 0.7152, 0.0722);

	// - Rotación
	vec2 newTexCoord = texCoord - vec2(0.5);
	newTexCoord *= rotationMatrix;
	newTexCoord += vec2(0.5);

	// - Hatching

	// - Obtener intensidad de color del fragmento
	float brightnessIntensity = dot(color, luminance);
	color = hatching(newTexCoord, brightnessIntensity);

	FragColor = vec4(color, 1.0);
}