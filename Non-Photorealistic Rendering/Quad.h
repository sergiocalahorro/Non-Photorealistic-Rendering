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

	// - Geometr�a
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> texCoords;

	// - VAO
	VAO *vao;

	// - T�cnica Halftone
	HalftoneTechnique halftone;
	HalftoneTechnique initialHalftone;

	// - T�cnica Dithering
	DitheringTechnique dithering;
	DitheringTechnique initialDithering;

	// - T�cnica Charcoal
	CharcoalTechnique charcoal;
	CharcoalTechnique initialCharcoal;

	// - T�cnica Painterly
	PainterlyTechnique painterly;
	PainterlyTechnique initialPainterly;

	// - T�cnica Pixel Art
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

	// - Configuraci�n de la t�cnica Halftone
	void setHalftoneTechnique(HalftoneTechnique halftone);
	HalftoneTechnique& getHalftoneTechnique();
	void resetHalftoneTechnique();

	// - Configuraci�n de la t�cnica Dithering
	void setDitheringTechnique(DitheringTechnique dithering);
	DitheringTechnique& getDitheringTechnique();
	void resetDitheringTechnique();

	// - Configuraci�n de la t�cnica Charcoal
	void setCharcoalTechnique(CharcoalTechnique charcoal);
	CharcoalTechnique& getCharcoalTechnique();
	void resetCharcoalTechnique();

	// - Configuraci�n de la t�cnica Painterly
	void setPainterlyTechnique(PainterlyTechnique painterly);
	PainterlyTechnique& getPainterlyTechnique();
	void resetPainterlyTechnique();

	// - Configuraci�n de la t�cnica Pixel Art
	void setPixelArtTechnique(PixelArtTechnique pixelArt);
	PixelArtTechnique& getPixelArtTechnique();
	void resetPixelArtTechnique();
};