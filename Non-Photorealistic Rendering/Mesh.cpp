#include "Mesh.h"

// - Constructor
Mesh::Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		   std::vector<unsigned int> topology, std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->texCoords = texCoords;
	this->topology = topology;
	this->textures = textures;

	// - Creación de VAO y VBOs
	vao = new VAO();
	vao->fillVBO(this->vertices, this->texCoords);
}

// - Constructor (con tangentes)
Mesh::Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		   std::vector<glm::vec3> tangents, std::vector<unsigned int> topology,
		   std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->texCoords = texCoords;
	this->tangents = tangents;
	this->topology = topology;
	this->textures = textures;

	// - Creación de VAO y VBOs
	vao = new VAO();
	vao->fillVBO(this->vertices, this->texCoords, this->tangents);
}

// - Constructor (con tangentes y bitangentes)
Mesh::Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		   std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents, 
		   std::vector<unsigned int> topology, std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->texCoords = texCoords;
	this->tangents = tangents;
	this->bitangents = bitangents;
	this->topology = topology;
	this->textures = textures;

	// - Creación de VAO y VBOs
	vao = new VAO();
	vao->fillVBO(this->vertices, this->texCoords, this->tangents, this->bitangents);
}

// - Constructor (con índices de adyacencia)
Mesh::Mesh(std::vector<PosNorm> vertices, std::vector<glm::vec2> texCoords,
		   std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents,
		   std::vector<unsigned int> topology, std::vector<unsigned int> adjacencyIndices,
		   std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->texCoords = texCoords;
	this->tangents = tangents;
	this->bitangents = bitangents;
	this->topology = topology;
	this->adjacencyIndices = adjacencyIndices;
	this->textures = textures;

	// - Creación de VAO y VBOs
	vao = new VAO();
	vao->fillVBO(this->vertices, this->texCoords, this->tangents, this->bitangents);
}

// - Destructor
Mesh::~Mesh()
{
	vertices.clear();
	texCoords.clear();
	tangents.clear();
	bitangents.clear();
	topology.clear();
	textures.clear();
}

// - Obtener índices de topología
std::vector<unsigned int> Mesh::getTopology()
{
	return topology;
}

// - Obtener índices de topología
std::vector<unsigned int> Mesh::getAdjacencyIndices()
{
	return adjacencyIndices;
}

// - Asignar índices de adyacencias
void Mesh::setAdjacencyIndices(std::vector<unsigned int> adjacencyIndices)
{
	this->adjacencyIndices = adjacencyIndices;
}

// - Aplicar texturas a los shaders
void Mesh::applyTextures(ShaderProgram &shader)
{
	for (int i = 0; i < textures.size(); i++)
	{
		std::string sampler;
		std::string type = textures[i]->getType();

		if (type == "textureDiffuse")
		{
			sampler = "TexSamplerDiffuse";
		}
		else if (type == "textureSpecular")
		{
			sampler = "TexSamplerSpecular";
		}
		else if (type == "textureNormal")
		{
			sampler = "TexSamplerNormal";
		}

		// - Asignar el sampleador de textura correspondiente
		if (!sampler.empty())
		{
			shader.setUniform(sampler, i);
		}

		// - Enlazar textura con la unidad de textura de GPU correspondiente
		textures[i]->bindTexture(GL_TEXTURE_2D, i);
	}
}

// - Aplicar textura difusa
void Mesh::applyDiffuseTexture(ShaderProgram &shader)
{
	for (int i = 0; i < textures.size(); i++)
	{
		std::string type = textures[i]->getType();

		if (type == "textureDiffuse")
		{
			shader.setUniform("TexSamplerDiffuse", i);
		}

		// - Enlazar textura con la unidad de textura de GPU correspondiente
		textures[i]->bindTexture(GL_TEXTURE_2D, i);
	}
}

// - Dibujar la malla
void Mesh::draw(ShaderProgram &shader)
{
	// - Dibujar la malla de triángulos
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLES, topology);
}

// - Dibujar la malla usando texturas
void Mesh::drawWithTextures(ShaderProgram &shader)
{
	applyTextures(shader);

	// - Dibujar la malla de triángulos
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLES, topology);
}

// - Dibujar la malla usando sólo textura difusa
void Mesh::drawWithDiffuseTexture(ShaderProgram &shader)
{
	applyDiffuseTexture(shader);

	// - Dibujar la malla de triángulos
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLES, topology);
}

// - Dibujar contorno avanzado de la malla
void Mesh::drawAdvancedOutline(ShaderProgram &shader)
{
	// - Dibujar la malla de triángulos
	vao->fillIBOAdjacencies(adjacencyIndices);
	vao->draw(GL_TRIANGLES_ADJACENCY, adjacencyIndices);
}