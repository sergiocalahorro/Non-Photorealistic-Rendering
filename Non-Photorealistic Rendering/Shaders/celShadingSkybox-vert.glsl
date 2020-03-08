#version 400

layout (location = 0) in vec3 vPosition;

uniform mat4 mViewProj;

out vec3 texCoord;

void main()
{
    texCoord = vPosition;
	vec4 position = mViewProj * vec4(vPosition, 1.0);
	gl_Position = position.xyww;
}  