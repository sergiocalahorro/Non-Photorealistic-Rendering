#pragma once

#include "Element3D.h"
#include "Structures.h"
#include "VAO.h"
#include "Material.h"
#include "Texture.h"

#include <vector>

class Plane: public Element3D
{
private:
	// - Anchura y altura
	int width;
	int height;

	// - Tiling
	int tilingHorizontal;
	int tilingVertical;

	// - Geometría
	std::vector<PosNorm> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> tangents;

	// - Topología
	std::vector<GLuint> topology;

	// - Textura
	Texture* texture;

	// - Texturas de hatching
	Texture *hatchBright;
	Texture *hatchDark;

	// - VAO
	VAO *vao;

public:
	// - Constructor
	Plane(int width, int height, int tilingHorizontal, int tilingVertical);

	// - Destructor
	~Plane();

	// - Cargar textura
	void loadTexture(std::string filename);

	// - Cargar texturas de hatching
	void setHatchingTextures(std::string dark, std::string bright);

	// - Dibujar el plano de distintas formas
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