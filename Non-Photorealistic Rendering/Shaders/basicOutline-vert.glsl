#version 400

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 mModelView;
uniform mat4 mProjection;
uniform mat4 mvpMatrix;
uniform float outlineThickness;

void main() 
{
	vec4 position = mModelView * vec4(vPosition, 1.0);
	vec3 normal = normalize(vec3(mvpMatrix * vec4(vNormal, 0.0)));

	// - Modificar posición del vértice utilizando su normal y el grosor del contorno
	position.xyz += normal * outlineThickness;

	// - Hacer que la relación de tamaño del contorno se mantenga igual en coordenadas de pantalla
	vec2 offset = normalize(normal.xy) * outlineThickness * position.w;
	position.xy += offset;

	gl_Position = mProjection * position;
}