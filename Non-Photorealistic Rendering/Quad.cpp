#include "Quad.h"

// - Constructor
Quad::Quad()
{
	// - V�rtices
	vertices.push_back(glm::vec2(-1.f, -1.f));
	vertices.push_back(glm::vec2(1.f, -1.f));
	vertices.push_back(glm::vec2(-1.f, 1.f));

	vertices.push_back(glm::vec2(-1.f, 1.f));
	vertices.push_back(glm::vec2(1.f, -1.f));
	vertices.push_back(glm::vec2(1.f, 1.f));

	// - Coordenadas de textura
	texCoords.push_back(glm::vec2(0.f, 0.f));
	texCoords.push_back(glm::vec2(1.f, 0.f));
	texCoords.push_back(glm::vec2(0.f, 1.f));

	texCoords.push_back(glm::vec2(0.f, 1.f));
	texCoords.push_back(glm::vec2(1.f, 0.f));
	texCoords.push_back(glm::vec2(1.f, 1.f));

	// - Creaci�n del VAO y asignaci�n de geometr�a
	vao = new VAO();
	vao->fillVBOQuad(vertices, texCoords);
}

// - Destructor
Quad::~Quad()
{
	delete vao;
	vertices.clear();
	texCoords.clear();
}

// - Dibujar la escena usando la t�cnica Halftone
void Quad::drawHalftone(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("threshold", halftone.threshold);
	shader.setUniform("colorHalftone", halftone.color);
	shader.setUniform("intensityFactor", halftone.intensity);
	shader.setUniform("halftoneSize", halftone.size);

	vao->draw(vertices.size());
}

// - Dibujar la escena usando la t�cnica Dithering
void Quad::drawDithering(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("threshold1", dithering.threshold1);
	shader.setUniform("threshold2", dithering.threshold2);
	shader.setUniform("threshold3", dithering.threshold3);
	shader.setUniform("threshold4", dithering.threshold4);
	shader.setUniform("intensityThreshold1", dithering.intensityThreshold1);
	shader.setUniform("intensityThreshold2", dithering.intensityThreshold2);
	shader.setUniform("intensityThreshold3", dithering.intensityThreshold3);
	shader.setUniform("intensityThreshold4", dithering.intensityThreshold4);
	shader.setUniform("density", dithering.density);
	shader.setUniform("width", dithering.width);
	shader.setUniform("useSceneColor", dithering.useSceneColor);

	vao->draw(vertices.size());
}

// - Dibujar la escena usando la t�cnica Charcoal
void Quad::drawCharcoal(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("enableSobel", charcoal.sobelFilter);
	shader.setUniform("threshold", charcoal.threshold);
	shader.setUniform("edgeColor", charcoal.edgeColor);
	shader.setUniform("charcoalColor", glm::vec3(0.1f) * charcoal.colorMultiplier);
	shader.setUniform("noiseAmount", charcoal.noise);

	vao->draw(vertices.size());
}

// - Dibujar la escena usando la t�cnica Painterly
void Quad::drawPainterly(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("brushSize", painterly.brushSize);

	vao->draw(vertices.size());
}

// - Dibujar la escena de forma pixelada (8-bit o Pixel art)
void Quad::drawPixelArt(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("numHorizontalPixels", pixelArt.numHorizontalPixels);
	shader.setUniform("numVerticalPixels", pixelArt.numVerticalPixels);

	vao->draw(vertices.size());
}

// - Asignar par�metros de la t�cnica Halftone
void Quad::setHalftoneTechnique(HalftoneTechnique halftone)
{
	this->halftone = halftone;
	initialHalftone = this->halftone;
}

// - Obtener par�metros de la t�cnica Halftone
HalftoneTechnique& Quad::getHalftoneTechnique()
{
	return this->halftone;
}

// - Restaurar t�cnica Halftone a su estado por defecto
void Quad::resetHalftoneTechnique()
{
	this->halftone = initialHalftone;
}

// - Asignar par�metros de la t�cnica Dithering
void Quad::setDitheringTechnique(DitheringTechnique dithering)
{
	this->dithering = dithering;
	initialDithering = this->dithering;
}

// - Obtener par�metros de la t�cnica Dithering
DitheringTechnique& Quad::getDitheringTechnique()
{
	return this->dithering;
}

// - Restaurar t�cnica Dithering a su estado por defecto
void Quad::resetDitheringTechnique()
{
	this->dithering = initialDithering;
}

// - Asignar par�metros de la t�cnica Charcoal
void Quad::setCharcoalTechnique(CharcoalTechnique charcoal)
{
	this->charcoal = charcoal;
	initialCharcoal = this->charcoal;
}

// - Obtener par�metros de la t�cnica Charcoal
CharcoalTechnique& Quad::getCharcoalTechnique()
{
	return this->charcoal;
}

// - Restaurar t�cnica Charcoal a su estado por defecto
void Quad::resetCharcoalTechnique()
{
	this->charcoal = initialCharcoal;
}

// - Asignar par�metros de la t�cnica Painterly
void Quad::setPainterlyTechnique(PainterlyTechnique painterly)
{
	this->painterly = painterly;
	initialPainterly = this->painterly;
}

// - Obtener par�metros de la t�cnica Painterly
PainterlyTechnique& Quad::getPainterlyTechnique()
{
	return this->painterly;
}

// - Restaurar t�cnica Painterly a su estado por defecto
void Quad::resetPainterlyTechnique()
{
	this->painterly = initialPainterly;
}

// - Asignar par�metros de la t�cnica Pixel Art
void Quad::setPixelArtTechnique(PixelArtTechnique pixelArt)
{
	this->pixelArt = pixelArt;
	initialPixelArt = this->pixelArt;
}

// - Obtener par�metros de la t�cnica Pixel Art
PixelArtTechnique& Quad::getPixelArtTechnique()
{
	return this->pixelArt;
}

// - Restaurar t�cnica Pixel Art a su estado por defecto
void Quad::resetPixelArtTechnique()
{
	this->pixelArt = initialPixelArt;
}