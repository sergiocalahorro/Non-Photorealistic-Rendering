#include "VAO.h"

// - Constructor
VAO::VAO()
{
	// - Se genera el VAO
	vao = 0;
	glGenVertexArrays(1, &vao);

	// - Se generan los VBOs
	glGenBuffers(4, vbo);

	// - Se generan los IBOs
	glGenBuffers(2, ibo);
	}

// - Destructor
VAO::~VAO()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(4, vbo);
	glDeleteBuffers(2, ibo);
}

// - Crear VBO (posiciones)
void VAO::fillVBO(std::vector<glm::vec3> positions)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - 1) VBO (Posiciones y normales)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[0]);

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las posiciones
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec3), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (posiciones)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(),
				 positions.data(), GL_STATIC_DRAW);
}		

// - Crear VBO (posiciones, normales y coordenadas de textura)
void VAO::fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - 1) VBO (Posiciones y normales)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[0]);

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las posiciones
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (0)));

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 1) en el shader, en concreto la normal
	glEnableVertexAttribArray(1);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las normales
	glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (sizeof(glm::vec3))));

	// - Pasar al VBO el array que contiene la información (posiciones y normales)
	glBufferData(GL_ARRAY_BUFFER, sizeof(PosNorm) * posAndNorms.size(),
				 posAndNorms.data(), GL_STATIC_DRAW);

	// - 2) VBO (Coordenadas de textura)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[1]);

	// - Aquí se indica que los elementos del array de coordenadas de textura van asociados
	//   con el layout (location = 2) en el shader
	glEnableVertexAttribArray(2);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las coordenadas de textura
	glVertexAttribPointer(2, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (coordenadas de textura)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
				 texCoords.data(), GL_STATIC_DRAW);
}

// - Crear VBO (posiciones, normales, coordenadas de textura y tangentes)
void VAO::fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords,
					 std::vector<glm::vec3> tangents)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - 1) VBO (Posiciones y normales)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[0]);

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las posiciones
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (0)));

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 1) en el shader, en concreto la normal
	glEnableVertexAttribArray(1);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las normales
	glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (sizeof(glm::vec3))));

	// - Pasar al VBO el array que contiene la información (posiciones y normales)
	glBufferData(GL_ARRAY_BUFFER, sizeof(PosNorm) * posAndNorms.size(),
				 posAndNorms.data(), GL_STATIC_DRAW);

	// - 2) VBO (Coordenadas de textura)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[1]);

	// - Aquí se indica que los elementos del array de coordenadas de textura van asociados
	//   con el layout (location = 2) en el shader
	glEnableVertexAttribArray(2);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las coordenadas de textura
	glVertexAttribPointer(2, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (coordenadas de textura)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
				 texCoords.data(), GL_STATIC_DRAW);

	// - 3) VBO (Tangentes)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[2]);

	// - Aquí se indica que los elementos del array de tangentes van asociados con el
	//   layout (location = 3) en el shader
	glEnableVertexAttribArray(3);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las tangentes
	glVertexAttribPointer(3, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec3), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (tangentes)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tangents.size(),
				 tangents.data(), GL_STATIC_DRAW);
}

// - Crear VBO (posiciones, normales, coordenadas de textura, tangentes y bitangentes)
void VAO::fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords,
					 std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - 1) VBO (Posiciones y normales)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[0]);

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las posiciones
	glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (0)));

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 1) en el shader, en concreto la normal
	glEnableVertexAttribArray(1);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las normales
	glVertexAttribPointer(1, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(PosNorm), ((GLubyte *)NULL + (sizeof(glm::vec3))));

	// - Pasar al VBO el array que contiene la información (posiciones y normales)
	glBufferData(GL_ARRAY_BUFFER, sizeof(PosNorm) * posAndNorms.size(),
				 posAndNorms.data(), GL_STATIC_DRAW);

	// - 2) VBO (Coordenadas de textura)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[1]);

	// - Aquí se indica que los elementos del array de coordenadas de textura van asociados
	//   con el layout (location = 2) en el shader
	glEnableVertexAttribArray(2);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las coordenadas de textura
	glVertexAttribPointer(2, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (coordenadas de textura)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
				  texCoords.data(), GL_STATIC_DRAW);

	// - 3) VBO (Tangentes)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[2]);

	// - Aquí se indica que los elementos del array de tangentes van asociados con el
	//   layout (location = 3) en el shader
	glEnableVertexAttribArray(3);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las tangentes
	glVertexAttribPointer(3, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec3), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (tangentes)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tangents.size(),
				 tangents.data(), GL_STATIC_DRAW);

	// - 4) VBO (Bitangentes)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[3]);

	// - Aquí se indica que los elementos del array de bitangentes van asociados con el
	//   layout (location = 4) en el shader
	glEnableVertexAttribArray(4);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las bitangentes
	glVertexAttribPointer(4, sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec3), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (tangentes)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * bitangents.size(),
				 bitangents.data(), GL_STATIC_DRAW);
}

// - Crear VBO del Quad (rendering a textura)
void VAO::fillVBOQuad(std::vector<glm::vec2> vertices, std::vector<glm::vec2> texCoords)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - 1) VBO (Vértices)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[0]);

	// - Aquí se indica que uno de los elementos del array entrelazado va asociado con el
	//   layout (location = 0) en el shader, en concreto la posición
	glEnableVertexAttribArray(0);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las posiciones
	glVertexAttribPointer(0, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (vértices)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(),
				 vertices.data(), GL_STATIC_DRAW);

	// - 2) VBO (Coordenadas de textura)

	// - Activar VBO
	glBindBuffer(GLenum(GL_ARRAY_BUFFER), vbo[1]);

	// - Aquí se indica que los elementos del array de coordenadas de textura van asociados
	//   con el layout (location = 1) en el shader
	glEnableVertexAttribArray(1);

	// - Aquí se describen las características del puntero que permite a la GPU acceder a
	//   las coordenadas de textura
	glVertexAttribPointer(1, sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE,
						  sizeof(glm::vec2), ((GLubyte *)NULL + (0)));

	// - Pasar al VBO el array que contiene la información (coordenadas de textura)
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
				 texCoords.data(), GL_STATIC_DRAW);
}

// - Crear IBO (malla de triángulos)
void VAO::fillIBO(std::vector<GLuint> indices)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - Activar IBO (malla de triángulos)
	glBindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), ibo[0]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
				 indices.data(), GL_STATIC_DRAW);
}

// - Crear IBO (adyacencia de triángulos)
void VAO::fillIBOAdjacencies(std::vector<GLuint> indices)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);

	// - Activar IBO (malla de triángulos)
	glBindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), ibo[1]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
				 indices.data(), GL_STATIC_DRAW);						 
}

// - Dibujar triángulos
void VAO::draw(unsigned int numIndices)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numIndices);
}

// - Dibujar los elementos según el modo y la topología especificados
void VAO::draw(GLenum mode, std::vector<GLuint> indices)
{
	// - Siempre que se quiere usar un VAO, hay que activarlo con esta orden
	glBindVertexArray(vao);
	glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, NULL);
}