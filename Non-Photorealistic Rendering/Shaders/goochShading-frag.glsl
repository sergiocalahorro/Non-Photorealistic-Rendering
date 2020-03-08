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

// - Samplers de texturas
uniform sampler2D TexSamplerDiffuse;
uniform sampler2D TexSamplerSpecular;

// - Técnica Gooch Shading
uniform vec3 Kblue;
uniform vec3 Kyellow;
uniform float alpha;
uniform float beta;

// - Subrutina para seleccionar el tipo de fuente luminosa a utilizar
subroutine vec3 LightFunction(vec4);
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
vec3 AmbientLight(vec4 texDiffuse)
{
	vec3 Kad;

	if (texDiffuse.rgb == vec3(0.0))
	{
		// - Asignar color ambiente
		Kad = KaMaterial;
	} 
	else
	{
		// - Asignar color ambiente de la textura
		Kad = texDiffuse.rgb;
	}

	return (Ia * Kad);
}

// - Calcular fuente luminosa puntual
subroutine(LightFunction)
vec3 PointLight(vec4 texDiffuse) 
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
	vec3 Kad;

	// - Si no hay textura de la que obtener el color difuso, se usa el del material
	if (texDiffuse.rgb == vec3(0.f))
	{
		// - Obtener color ambiente y difuso del material
		Kad = KdMaterial;
	}
	else
	{
		// - Obtener color ambiente y difuso de la textura
		Kad = texDiffuse.rgb;
	}

	// - Componente difusa
	float dotLN = dot(l, n);
	float cosine = max(dotLN, 0.0);

	// - Técnica NPR: Gooch Shading

	// - Calcular colores frío y cálido
	vec3 Kcool = Kblue + alpha * Kad;
	vec3 Kwarm = Kyellow + beta * Kad;

	// - Calcular color difuso
	vec3 diffuse = Id * (((1.0 + cosine) / 2) * Kcool + (1 - ((1.0 + cosine) / 2)) * Kwarm);

	// - Color especular
	vec3 Ks;
	vec4 texSpecular = texture(TexSamplerSpecular, texCoord);
	
	// - Si no hay textura de la que obtener el color especular, se usa el del material
	if (texSpecular.rgb == vec3(0.f))
	{
		// - Obtener color especular del material
		Ks = KsMaterial;
	}
	else
	{
		// - Obtener color especular de la textura
		Ks = texSpecular.rgb;
	}

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dotLN < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color;

	// - Colorear el fragmento usando las componentes difusa y especular 
	color = fatt() * (diffuse + specular);

	return color;
}

// - Calcular fuente luminosa direccional
subroutine(LightFunction)
vec3 DirectionalLight(vec4 texDiffuse) 
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
	vec3 Kad;

	// - Si no hay textura de la que obtener el color difuso, se usa el del material
	if (texDiffuse.rgb == vec3(0.f))
	{
		// - Obtener color ambiente y difuso del material
		Kad = KdMaterial;
	}
	else
	{
		// - Obtener color ambiente y difuso de la textura
		Kad = texDiffuse.rgb;
	}

	// - Componente difusa
	float dotLN = dot(l, n);
	float cosine = max(dotLN, 0.0);

	// - Técnica NPR: Gooch Shading

	// - Calcular colores frío y cálido
	vec3 Kcool = Kblue + alpha * Kad;
	vec3 Kwarm = Kyellow + beta * Kad;

	// - Calcular color difuso
	vec3 diffuse = Id * (((1.0 + cosine) / 2) * Kcool + (1 - ((1.0 + cosine) / 2)) * Kwarm);

	// - Obtener color especular de la textura
	vec4 texSpecular = texture(TexSamplerSpecular, texCoord);
	vec3 Ks;

	// - Si no hay textura de la que obtener el color especular, se usa el del material
	if (texSpecular.rgb == vec3(0.f))
	{
		// - Obtener color especular del material
		Ks = KsMaterial;
	}
	else
	{
		// - Obtener color especular de la textura
		Ks = texSpecular.rgb;
	}

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dotLN < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color;

	// - Colorear el fragmento usando las componentes difusa y especular 
	color = (diffuse + specular);

	return color;
}

// - Calcular fuente luminosa spot
subroutine(LightFunction)
vec3 SpotLight(vec4 texDiffuse) 
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
	vec3 Kad;

	// - Si no hay textura de la que obtener el color difuso, se usa el del material
	if (texDiffuse.rgb == vec3(0.f))
	{
		// - Obtener color ambiente y difuso del material
		Kad = KdMaterial;
	}
	else
	{
		// - Obtener color ambiente y difuso de la textura
		Kad = texDiffuse.rgb;
	}

	// - Componente difusa
	float dotLN = dot(l, n);
	float cosine = max(dotLN, 0.0);

	// - Técnica NPR: Gooch Shading

	// - Calcular colores frío y cálido
	vec3 Kcool = Kblue + alpha * Kad;
	vec3 Kwarm = Kyellow + beta * Kad;

	// - Calcular color difuso
	vec3 diffuse = Id * (((1.0 + cosine) / 2) * Kcool + (1 - ((1.0 + cosine) / 2)) * Kwarm);

	// - Obtener color especular de la textura
	vec4 texSpecular = texture(TexSamplerSpecular, texCoord);
	vec3 Ks;

	// - Si no hay textura de la que obtener el color especular, se usa el del material
	if (texSpecular.rgb == vec3(0.f))
	{
		// - Obtener color especular del material
		Ks = KsMaterial;
	}
	else
	{
		// - Obtener color especular de la textura
		Ks = texSpecular.rgb;
	}

	// - Componente especular
	float specularFactor = pow(max(dot(r, v), 0.0), shininess);
	vec3 specular;

	// - Evitar que aparezca brillo especular si se ilumina desde detrás de la superficie
	if (dotLN < 0.0) 
	{
		specular = vec3(0.0);
	} 
	else 
	{
		specular = (Is * Ks * specularFactor);
	}

	// - Calcular color final
	vec3 color;

	// - Colorear el fragmento usando las componentes difusa y especular 
	color = spotFactor * fatt() * (diffuse + specular);
	
	return color;
}

void main() 
{
	// - Samplear textura
	vec4 texDiffuse = texture(TexSamplerDiffuse, texCoord);

	// - Obtener color del fragmento aplicando la fuente luminosa seleccionada
	FragColor = vec4(LightUniform(texDiffuse), 1.0);
}