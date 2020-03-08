#version 400

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

// - Matriz de modelado y visión
uniform mat4 mModelView;

void main()
{
	gl_Position = mModelView * vec4(vPosition, 1.0);
}