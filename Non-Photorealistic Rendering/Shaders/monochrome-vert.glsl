#version 400

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 mvpMatrix;
uniform mat4 mModelView;

out vec3 position;
out vec3 normal;

void main() 
{
	normal = vec3(mModelView * vec4(vNormal, 0.0));
	position = vec3(mModelView * vec4(vPosition, 1.0));

	gl_Position = mvpMatrix * vec4(vPosition, 1.0);
}