#include "Quad.h"

// - Constructor
Quad::Quad()
{
	// - Vértices
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

	// - Creación del VAO y asignación de geometría
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

// - Dibujar la escena usando la técnica Halftone
void Quad::drawHalftone(ShaderProgram &shader, unsigned int textureUnit)
{
	shader.setUniform("TexScene", (int) textureUnit);
	shader.setUniform("threshold", halftone.threshold);
	shader.setUniform("colorHalftone", halftone.color);
	shader.setUniform("intensityFactor", halftone.intensity);
	shader.setUniform("halftoneSize", halftone.size);

	vao->draw(vertices.size());
}

// - Dibujar la escena usando la técnica Dithering
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

// - Dibujar la escena usando la técnica Charcoal
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

// - Dibujar la escena usando la técnica Painterly
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

// - Asignar parámetros de la técnica Halftone
void Quad::setHalftoneTechnique(HalftoneTechnique halftone)
{
	this->halftone = halftone;
	initialHalftone = this->halftone;
}

// - Obtener parámetros de la técnica Halftone
HalftoneTechnique& Quad::getHalftoneTechnique()
{
	return this->halftone;
}

// - Restaurar técnica Halftone a su estado por defecto
void Quad::resetHalftoneTechnique()
{
	this->halftone = initialHalftone;
}

// - Asignar parámetros de la técnica Dithering
void Quad::setDitheringTechnique(DitheringTechnique dithering)
{
	this->dithering = dithering;
	initialDithering = this->dithering;
}

// - Obtener parámetros de la técnica Dithering
DitheringTechnique& Quad::getDitheringTechnique()
{
	return this->dithering;
}

// - Restaurar técnica Dithering a su estado por defecto
void Quad::resetDitheringTechnique()
{
	this->dithering = initialDithering;
}

// - Asignar parámetros de la técnica Charcoal
void Quad::setCharcoalTechnique(CharcoalTechnique charcoal)
{
	this->charcoal = charcoal;
	initialCharcoal = this->charcoal;
}

// - Obtener parámetros de la técnica Charcoal
CharcoalTechnique& Quad::getCharcoalTechnique()
{
	return this->charcoal;
}

// - Restaurar técnica Charcoal a su estado por defecto
void Quad::resetCharcoalTechnique()
{
	this->charcoal = initialCharcoal;
}

// - Asignar parámetros de la técnica Painterly
void Quad::setPainterlyTechnique(PainterlyTechnique painterly)
{
	this->painterly = painterly;
	initialPainterly = this->painterly;
}

// - Obtener parámetros de la técnica Painterly
PainterlyTechnique& Quad::getPainterlyTechnique()
{
	return this->painterly;
}

// - Restaurar técnica Painterly a su estado por defecto
void Quad::resetPainterlyTechnique()
{
	this->painterly = initialPainterly;
}

// - Asignar parámetros de la técnica Pixel Art
void Quad::setPixelArtTechnique(PixelArtTechnique pixelArt)
{
	this->pixelArt = pixelArt;
	initialPixelArt = this->pixelArt;
}

// - Obtener parámetros de la técnica Pixel Art
PixelArtTechnique& Quad::getPixelArtTechnique()
{
	return this->pixelArt;
}

// - Restaurar técnica Pixel Art a su estado por defecto
void Quad::resetPixelArtTechnique()
{
	this->pixelArt = initialPixelArt;
}