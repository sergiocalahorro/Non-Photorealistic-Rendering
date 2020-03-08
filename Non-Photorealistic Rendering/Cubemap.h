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

	// - Geometr�a
	std::vector<glm::vec3> vertices;
	
	// - Im�genes y sus par�metros
	std::vector<std::vector<unsigned char>> images;
	std::vector<unsigned> widthImages;
	std::vector<unsigned> heightImages;

	// - VAO
	VAO *vao;

	// - T�cnica Cel-Shading
	CelShadingTechnique celShading;
	CelShadingTechnique initialCelShading;

	// - T�cnica Hatching
	HatchingTechnique hatching;
	HatchingTechnique initialHatching;

	Texture *hatchBright;
	Texture *hatchDark;

public:
	// - Constructor
	Cubemap();

	// - Destructor
	~Cubemap();

	// - Cargar im�genes de disco
	void loadTextures(std::vector<std::string> filenames);

	// - Dibujado del cubemap
	void drawRealistic(ShaderProgram &shader, glm::mat4 vp);
	void drawCelShading(ShaderProgram &shader, glm::mat4 vp, glm::vec3 Ia);
	void drawHatching(ShaderProgram &shader, glm::mat4 vp);

	// - Configuraci�n de la t�cnica Cel-Shading
	void setCelShadingTechnique(CelShadingTechnique celShading);
	CelShadingTechnique& getCelShadingTechnique();
	void resetCelShadingTechnique();

	// - Configuraci�n de la t�cnica Hatching
	void setHatchingTechnique(HatchingTechnique hatching);
	HatchingTechnique& getHatchingTechnique();
	void resetHatchingTechnique();

	void setHatchingTextures(std::string dark, std::string bright);
};