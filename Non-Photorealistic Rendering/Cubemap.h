#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <string>

#include "Texture.h"
#include "VAO.h"
#include "ShaderProgram.h"

class Cubemap
{
private:
	unsigned int texture;

	// - Geometría
	std::vector<glm::vec3> vertices;
	
	// - Imágenes y sus parámetros
	std::vector<std::vector<unsigned char>> images;
	std::vector<unsigned> widthImages;
	std::vector<unsigned> heightImages;

	// - VAO
	VAO *vao;

	// - Técnica Cel-Shading
	CelShadingTechnique celShading;
	CelShadingTechnique initialCelShading;

	// - Técnica Hatching
	HatchingTechnique hatching;
	HatchingTechnique initialHatching;

	Texture *hatchBright;
	Texture *hatchDark;

public:
	// - Constructor
	Cubemap();

	// - Destructor
	~Cubemap();

	// - Cargar imágenes de disco
	void loadTextures(std::vector<std::string> filenames);

	// - Dibujado del cubemap
	void drawRealistic(ShaderProgram &shader, glm::mat4 vp);
	void drawCelShading(ShaderProgram &shader, glm::mat4 vp, glm::vec3 Ia);
	void drawHatching(ShaderProgram &shader, glm::mat4 vp);

	// - Configuración de la técnica Cel-Shading
	void setCelShadingTechnique(CelShadingTechnique celShading);
	CelShadingTechnique& getCelShadingTechnique();
	void resetCelShadingTechnique();

	// - Configuración de la técnica Hatching
	void setHatchingTechnique(HatchingTechnique hatching);
	HatchingTechnique& getHatchingTechnique();
	void resetHatchingTechnique();

	void setHatchingTextures(std::string dark, std::string bright);
};