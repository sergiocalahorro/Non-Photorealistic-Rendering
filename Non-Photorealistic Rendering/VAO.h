#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>

#include "Structures.h"

class VAO
{
private:
	GLuint vao;
	GLuint vbo[4] = { 0, 0, 0, 0};
	GLuint ibo[2] = { 0, 0 };

public:
	// - Constructor
	VAO();

	// - Destructor
	~VAO();

	// - Rellenar VBOs
	void fillVBO(std::vector<glm::vec3> positions);

	void fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords);

	void fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords,
				 std::vector<glm::vec3> tangents);

	void fillVBO(std::vector<PosNorm> posAndNorms, std::vector<glm::vec2> texCoords,
				 std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents);

	void fillVBOQuad(std::vector<glm::vec2> vertices, std::vector<glm::vec2> texCoords);

	// - Rellenar IBOs
	void fillIBO(std::vector<GLuint> indices);
	void fillIBOAdjacencies(std::vector<GLuint> indices);

	// - Dibujado
	void draw(GLenum mode, std::vector<GLuint> indices);
	void draw(unsigned int numIndices);
};