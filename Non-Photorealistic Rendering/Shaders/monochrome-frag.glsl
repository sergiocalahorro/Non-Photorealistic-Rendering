#version 400

in vec3 position;
in vec3 normal;

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
	// - Asignar color ambiente
	vec3 Kad = KaMaterial;

	return (Ia * Kad);
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
	
	// - Color ambiente y difuso
	vec3 Kad = KdMaterial;

	// - Componente difusa
	vec3 diffuse = (Id * Kad * max(dot(l, n), 0.0));

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * pow(max(dot(r, v), 0.0), shininess));
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

	// - Color ambiente y difuso
	vec3 Kad = KdMaterial;

	// - Componente difusa
	vec3 diffuse = (Id * Kad * max(dot(l, n), 0.0));

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * pow(max(dot(r, v), 0.0), shininess));
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

	// - Color ambiente y difuso
	vec3 Kad = KdMaterial;

	// - Componente difusa
	vec3 diffuse = (Id * Kad * max(dot(l, n), 0.0));

	// - Color especular
	vec3 Ks = KsMaterial;

	// - Componente especular
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dot(l, n) < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * pow(max(dot(r, v), 0.0), shininess));
	}

	// - Calcular color final
	vec3 color = spotFactor * fatt() * (diffuse + specular);

	return color;
}

void main()
{
	FragColor = vec4(LightUniform(), 1.0);
}