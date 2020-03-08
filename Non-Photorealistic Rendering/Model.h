#pragma once

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Element3D.h"
#include "Mesh.h"
#include "Material.h"

class Model: public Element3D
{
private:
	// - Datos del modelo
	std::vector<Mesh*> meshes;
	std::string directory;

	// - Mapa de ejes (adyacencias)
	std::map<std::pair<unsigned int, unsigned int>, HalfEdge*> edges;

	// - Texturas cargadas
	std::vector<Texture*> loadedTextures;

	// - Texturas de hatching
	Texture *hatchBright;
	Texture *hatchDark;

	// - Cargar el modelo
	void loadModel(std::string path);

	// - Procesamiento de la escena a nivel de nodo
	void processNode(aiNode *node, const aiScene *scene);

	// - Procesamiento de la escena a nivel de malla
	Mesh* processMesh(aiMesh *mesh, const aiScene *scene);

	// - Carga de texturas
	std::vector<Texture*> loadMaterialTextures(aiMaterial *material, aiTextureType type,
											   std::string typeName);

	// - Carga de materiales
	void loadMaterial(aiMaterial *mat);

	// - Construir vector de índices con topología y los índices de los vértices adyacentes
	std::vector<unsigned int> buildTopologyPlusAdjacencies(std::vector<unsigned int> topology);

public:
	// - Constructor
	Model(std::string path);

	// - Destructor
	~Model();

	// - Cargar texturas de hatching
	void setHatchingTextures(std::string dark, std::string bright);
	
	// - Dibujar el modelo de distintas formas
	void drawRealistic(ShaderProgram &shader, glm::mat4 mModel,
					   glm::mat4 mView, glm::mat4 mProjection) override;

	void drawMonochrome(ShaderProgram &shader, glm::mat4 mModel,
						glm::mat4 mView, glm::mat4 mProjection) override;

	void drawCelShading(ShaderProgram &shader, glm::mat4 mModel,
						glm::mat4 mView, glm::mat4 mProjection) override;

	void drawHatching(ShaderProgram &shader, glm::mat4 mModel,
					  glm::mat4 mView, glm::mat4 mProjection) override;

	void drawGoochShading(ShaderProgram &shader, glm::mat4 mModel,
						  glm::mat4 mView, glm::mat4 mProjection) override;

	// - Dibujar contornos
	void drawBasicOutline(ShaderProgram &shader, glm::mat4 mModel,
						  glm::mat4 mView, glm::mat4 mProjection) override;

	void drawAdvancedOutline(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection) override;

};