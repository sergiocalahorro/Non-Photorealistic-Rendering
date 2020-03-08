#include "Cubemap.h"

// - Constructor
Cubemap::Cubemap()
{
	texture = 0;
	glGenTextures(1, &texture);

	// - Geometría del cubo

	// - Cara 1
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	// - Cara 2
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

	// - Cara 3
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	// - Cara 4
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

	// - Cara 5
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	// - Cara 6
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));

	vao = new VAO();
	vao->fillVBO(vertices);
}

// - Destructor
Cubemap::~Cubemap()
{
	glDeleteTextures(1, &texture);
}

// - Cargar imágenes de disco
void Cubemap::loadTextures(std::vector<std::string> filenames)
{
	images.resize(filenames.size());
	widthImages.resize(filenames.size());
	heightImages.resize(filenames.size());

	for (unsigned int i = 0; i < filenames.size(); i++)
	{
		Texture *texture = new Texture();
		texture->loadImage(filenames[i].c_str(), true);
		texture->bindTexture(GL_TEXTURE_2D, i);
		texture->defineTexture(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
							   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_LINEAR,
							   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T,
							   GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE,
							   0, GL_RGBA, texture->getWidth(),
							   texture->getHeight(), 0,
							   GL_RGBA, GL_UNSIGNED_BYTE, texture->getImage());

	}
}

// - Dibujar cubemap de forma realista
void Cubemap::drawRealistic(ShaderProgram &shader, glm::mat4 vp)
{
	// - Asignar uniforms correspondientes a la matriz de visión y proyección, y al sampleador de textura
	shader.setUniform("mViewProj", vp);
	shader.setUniform("SamplerSkybox", 0);

	vao->draw(vertices.size());
}

// - Dibujar cubemap utilizando la técnica Cel-Shading
void Cubemap::drawCelShading(ShaderProgram &shader, glm::mat4 vp, glm::vec3 Ia)
{
	// - Asignar uniforms correspondientes a la matriz de visión y proyección, y al sampleador de textura
	shader.setUniform("mViewProj", vp);
	shader.setUniform("SamplerSkybox", 0);
	shader.setUniform("Ia", Ia);
	shader.setUniform("tones", celShading.tones);

	vao->draw(vertices.size());
}

// - Dibujar cubemap utilizando la técnica Hatching
void Cubemap::drawHatching(ShaderProgram &shader, glm::mat4 vp)
{
	// - Asignar uniforms correspondientes a la matriz de visión y proyección, y al sampleador de textura
	shader.setUniform("mViewProj", vp);
	shader.setUniform("SamplerSkybox", 0);
	shader.setUniform("density", hatching.density);

	// - Ángulos de rotación
	float angleXRad = glm::radians(hatching.rotationAngleX);
	float angleYRad = glm::radians(hatching.rotationAngleY);
	float angleZRad = glm::radians(hatching.rotationAngleZ);

	// - Rotation X
	glm::mat3 rotationXMatrix(1.f, 0.f, 0.f,
							  0.f, glm::cos(angleXRad), -glm::sin(angleXRad),
							  0.f, glm::sin(angleXRad), glm::cos(angleXRad));

	// - Rotation Y
	glm::mat3 rotationYMatrix(glm::cos(angleYRad), 0.f, glm::sin(angleYRad),
							  0.f, 1.f, 0.f,
							 -glm::sin(angleYRad), 0.f, glm::cos(angleYRad));

	// - Rotation Z
	glm::mat3 rotationZMatrix(glm::cos(angleZRad), -glm::sin(angleZRad), 0.f,
							  glm::sin(angleZRad), glm::cos(angleZRad), 0.f,
							  0.f, 0.f, 1.f);

	shader.setUniform("rotationMatrix", rotationZMatrix * rotationYMatrix * rotationXMatrix);



	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchDark->bindTexture(GL_TEXTURE_2D, images.size());
	shader.setUniform("hatchDark", (int) (images.size()));

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchBright->bindTexture(GL_TEXTURE_2D, (images.size() + 1));
	shader.setUniform("hatchBright", (int) (images.size() + 1));

	vao->draw(vertices.size());
}

// - Asignar parámetros de la técnica Cel-Shading
void Cubemap::setCelShadingTechnique(CelShadingTechnique celShading)
{
	this->celShading = celShading;
	initialCelShading = this->celShading;
}

// - Obtener parámetros de la técnica Cel-Shading
CelShadingTechnique& Cubemap::getCelShadingTechnique()
{
	return this->celShading;
}

// - Restaurar técnica Cel-Shading a su estado por defecto
void Cubemap::resetCelShadingTechnique()
{
	this->celShading = initialCelShading;
}

// - Asignar parámetros de la técnica Hatching
void Cubemap::setHatchingTechnique(HatchingTechnique hatching)
{
	this->hatching = hatching;
	initialHatching = this->hatching;
}

// - Obtener parámetros de la técnica Hatching
HatchingTechnique& Cubemap::getHatchingTechnique()
{
	return this->hatching;
}

// - Restaurar técnica Hatching a su estado por defecto
void Cubemap::resetHatchingTechnique()
{
	this->hatching = initialHatching;
}

// - Asignar texturas de Hatching
void Cubemap::setHatchingTextures(std::string dark, std::string bright)
{
	hatchDark = new Texture();
	hatchDark->loadImage(dark.c_str());
	hatchDark->bindTexture(GL_TEXTURE_2D, images.size());
	hatchDark->defineTexture(GL_TEXTURE_2D,
							 GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							 GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							 0, GL_RGBA, hatchDark->getWidth(), hatchDark->getHeight(),
							 0, GL_RGBA, GL_UNSIGNED_BYTE, hatchDark->getImage());

	hatchBright = new Texture();
	hatchBright->loadImage(bright.c_str());
	hatchBright->bindTexture(GL_TEXTURE_2D, (images.size() + 1));
	hatchBright->defineTexture(GL_TEXTURE_2D,
							   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							   0, GL_RGBA, hatchBright->getWidth(), hatchBright->getHeight(),
							   0, GL_RGBA, GL_UNSIGNED_BYTE, hatchBright->getImage());
}