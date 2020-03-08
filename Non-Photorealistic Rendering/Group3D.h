#pragma once

#include "Element3D.h"
#include <vector>

class Group3D: public Element3D
{
private:
	// - Conjunto de elementos 3D
	std::vector<Element3D*> elements;

public:
	// - Constructor por defecto
	Group3D();

	// - Constructor
	Group3D(std::vector<Element3D*> elements);

	// - Destructor
	~Group3D();

	// - Añadir elemento
	void addElement(Element3D* element);

	// - Obtener elemento dada su posición
	Element3D* getElement(int pos);

	// - Obtener número de elementos del grupo
	int getNumElements();
	
	// - Métodos de dibujado
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