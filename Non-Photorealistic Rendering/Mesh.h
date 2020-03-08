#pragma once

#include <vector>

#include "Structures.h"
#include "VAO.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Mesh
{
private:
	// - VAO
	VAO *vao;

	// - Datos de la malla
	std::vector<PosNorm> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> topology;
	std::vector<Texture*> textures;

	std::vector<unsigned int> adjacencyIndices;

	// - Aplicar texturas a shaders
	void applyTextures(ShaderProgram &shader);
	void applyDiffuseTexture(ShaderProgram &shader);

public:
	// - Constructor
	Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		 std::vector<unsigned int> topology, std::vector<Texture*> textures);

	// - Constructor (con tangentes)
	Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		 std::vector<glm::vec3> tangents, std::vector<unsigned int> topology,
		 std::vector<Texture*> textures);

	// - Constructor (con tangentes y bitangentes)
	Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		 std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents, 
		 std::vector<unsigned int> topology, std::vector<Texture*> textures);

	// - Constructor (índices de adyacencia)
	Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		 std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents,
		 std::vector<unsigned int> topology, std::vector<unsigned int> adjacencyIndices,
		 std::vector<Texture*> textures);

	// - Destructor
	~Mesh();
	
	// - Getters y setters
	std::vector<unsigned int> getTopology();
	std::vector<unsigned int> getAdjacencyIndices();
	void setAdjacencyIndices(std::vector<unsigned int> adjacencyIndices);

	// - Dibujar la malla de distintas formas
	void draw(ShaderProgram &shader);
	void drawWithTextures(ShaderProgram &shader);
	void drawWithDiffuseTexture(ShaderProgram &shader);
	void drawAdvancedOutline(ShaderProgram &shader);
};