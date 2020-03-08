#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <string>

#include "VAO.h"
#include "ShaderProgram.h"

class Quad
{
private:
	unsigned int texture;

	// - Geometría
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> texCoords;

	// - VAO
	VAO *vao;

	// - Técnica Halftone
	HalftoneTechnique halftone;
	HalftoneTechnique initialHalftone;

	// - Técnica Dithering
	DitheringTechnique dithering;
	DitheringTechnique initialDithering;

	// - Técnica Charcoal
	CharcoalTechnique charcoal;
	CharcoalTechnique initialCharcoal;

	// - Técnica Painterly
	PainterlyTechnique painterly;
	PainterlyTechnique initialPainterly;

	// - Técnica Pixel Art
	PixelArtTechnique pixelArt;
	PixelArtTechnique initialPixelArt;

public:
	// - Constructor
	Quad();

	// - Destructor
	~Quad();

	// - Modos de dibujado de post-procesamiento
	void drawHalftone(ShaderProgram &shader, unsigned int textureUnit);
	void drawDithering(ShaderProgram &shader, unsigned int textureUnit);
	void drawCharcoal(ShaderProgram &shader, unsigned int textureUnit);
	void drawPainterly(ShaderProgram &shader, unsigned int textureUnit);
	void drawPixelArt(ShaderProgram &shader, unsigned int textureUnit);

	// - Configuración de la técnica Halftone
	void setHalftoneTechnique(HalftoneTechnique halftone);
	HalftoneTechnique& getHalftoneTechnique();
	void resetHalftoneTechnique();

	// - Configuración de la técnica Dithering
	void setDitheringTechnique(DitheringTechnique dithering);
	DitheringTechnique& getDitheringTechnique();
	void resetDitheringTechnique();

	// - Configuración de la técnica Charcoal
	void setCharcoalTechnique(CharcoalTechnique charcoal);
	CharcoalTechnique& getCharcoalTechnique();
	void resetCharcoalTechnique();

	// - Configuración de la técnica Painterly
	void setPainterlyTechnique(PainterlyTechnique painterly);
	PainterlyTechnique& getPainterlyTechnique();
	void resetPainterlyTechnique();

	// - Configuración de la técnica Pixel Art
	void setPixelArtTechnique(PixelArtTechnique pixelArt);
	PixelArtTechnique& getPixelArtTechnique();
	void resetPixelArtTechnique();
};