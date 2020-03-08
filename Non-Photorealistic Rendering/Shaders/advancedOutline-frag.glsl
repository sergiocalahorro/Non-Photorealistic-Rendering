#version 400

in float dist;

uniform float outlineThickness; // - Grosor de la silueta (mitad)
uniform vec3 outlineColor; // - Color de la silueta

layout(location = 0) out vec4 FragColor;

void main()
{
	float alpha = 1.0;
	float absDist = abs(dist);
	float tipLength = 2.0 * fwidth(absDist);

	if (absDist > outlineThickness - tipLength)
	{
		alpha = 1.0 - (absDist - outlineThickness + tipLength) / tipLength; 
	}

    FragColor = vec4(outlineColor, alpha);
}