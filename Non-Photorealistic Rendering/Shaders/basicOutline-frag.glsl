#version 400

uniform vec3 outlineColor;

out vec4 FragColor;

void main() 
{
	FragColor = vec4(outlineColor, 1.0);
}