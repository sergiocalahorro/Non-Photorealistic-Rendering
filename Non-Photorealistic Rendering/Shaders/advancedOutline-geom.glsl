#version 400

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 15) out;

// - Matriz de proyección
uniform mat4 mProjection;

// - Grosor de la silueta
uniform float outlineThickness; 

// - Factor utilizado para extender las tiras de triángulos y juntar las líneas que forman la silueta
uniform float lineExtension; 

out float dist;

// - Determinar si la cara del triángulo es visible calculando su área
bool isFrontFacing(vec3 a, vec3 b, vec3 c)
{
	float area = (a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y);

	if (area > 0)
	{
		return true;
	}

	return false;
}

// - Crear la primitiva del eje (serán parte del contorno las aristas compartidas por un triángulo cuya cara
//   sea frontal y otro triángulo cuya cara sea trasera)
void emitEdge(vec3 v0, vec3 v1)
{
	vec3 vector = lineExtension * vec3(v1.xy - v0.xy, 0.0);
	vec3 extrusion = vec3(-normalize(vector).y, normalize(vector).x, 0.0) * outlineThickness;

	dist = outlineThickness; 
    gl_Position = mProjection * vec4(v0 - extrusion - vector, 1.0); 
	EmitVertex();

	dist = -outlineThickness; 
 	gl_Position = mProjection * vec4(v0 + extrusion - vector, 1.0); 
	EmitVertex();

	dist = outlineThickness; 
 	gl_Position = mProjection * vec4(v1 - extrusion + vector, 1.0);
	EmitVertex();

	dist = -outlineThickness; 
    gl_Position = mProjection * vec4(v1 + extrusion + vector, 1.0); 
	EmitVertex();

    EndPrimitive();
}

void main()
{
	vec3 v0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
	vec3 v1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
	vec3 v2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;
	vec3 v3 = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;
	vec3 v4 = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;
	vec3 v5 = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;

	if (isFrontFacing(v0, v2, v4)) 
	{
		if (!isFrontFacing(v0, v1, v2)) 
		{
			emitEdge(v0, v2);
		}

		if (!isFrontFacing(v2, v3, v4)) 
		{
			emitEdge(v2, v4);
		}

		if (!isFrontFacing(v4, v5, v0)) 
		{
			emitEdge(v4, v0);
		}
	}
}