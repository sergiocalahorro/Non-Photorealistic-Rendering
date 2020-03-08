#include "Group3D.h"
#include <gtc/matrix_transform.hpp>

// - Constructor por defecto
Group3D::Group3D()
{
	// - No es un plano
	isPlane = false;
}

// - Constructor
Group3D::Group3D(std::vector<Element3D*> elements)
{
	this->elements = elements;
}

// - Destructor
Group3D::~Group3D()
{
	for (Element3D *element : elements)
	{
		delete element;
	}
	elements.clear();
}

// - Añadir elemento
void Group3D::addElement(Element3D *element)
{
	elements.push_back(element);
}

// - Obtener elemento dada su posición
Element3D* Group3D::getElement(int pos)
{
	return elements[pos];
}

// - Obtener número de elementos del grupo
int Group3D::getNumElements()
{
	return elements.size();
}

// - Dibujado del grupo 3D de forma realista
void Group3D::drawRealistic(ShaderProgram &shader, glm::mat4 mModel,
							glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawRealistic(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del grupo 3D de forma monocromática (material)
void Group3D::drawMonochrome(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawMonochrome(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del grupo 3D utilizando la técnica Cel-Shading
void Group3D::drawCelShading(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawCelShading(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del grupo 3D utilizando la técnica Hatching
void Group3D::drawHatching(ShaderProgram &shader, glm::mat4 mModel,
						   glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawHatching(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del grupo 3D utilizando la técnica Gooch Shading
void Group3D::drawGoochShading(ShaderProgram &shader, glm::mat4 mModel,
							   glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawGoochShading(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del contorno del grupo 3D
void Group3D::drawBasicOutline(ShaderProgram &shader, glm::mat4 mModel,
						  glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawBasicOutline(shader, mModel * elements[i]->getModelMatrix(), mView, mProjection);
	}
}

// - Dibujado del contorno avanzado del grupo 3D
void Group3D::drawAdvancedOutline(ShaderProgram &shader, glm::mat4 mModel,
								  glm::mat4 mView, glm::mat4 mProjection)
{
	for (int i = 0; i < elements.size(); i++)
	{
		// - Dibujar elemento
		elements[i]->drawAdvancedOutline(shader, mModel * elements[i]->getModelMatrix(),
										 mView, mProjection);
	}
}