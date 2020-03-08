#include "Element3D.h"

// - Constructor por defecto
Element3D::Element3D()
{
	modelMatrix = glm::mat4(1.0f);
}

// - Destructor
Element3D::~Element3D()
{

}

// - Obtener matriz de modelado del elemento 3D
glm::mat4 Element3D::getModelMatrix()
{
	return modelMatrix;
}

// - Asignar matriz de modelado del elemento 3D
void Element3D::setModelMatrix(glm::mat4 modelMatrix)
{
	this->modelMatrix = modelMatrix;
}

// - Transformaci�n geom�trica: traslaci�n
void Element3D::translate(glm::vec3 translation)
{
	this->modelMatrix = glm::translate(this->modelMatrix, translation);
}

// - Transformaci�n geom�trica: escalado
void Element3D::scale(glm::vec3 scale)
{
	this->modelMatrix = glm::scale(this->modelMatrix, scale);
}

// - Transformaci�n geom�trica: rotaci�n
void Element3D::rotate(glm::vec3 rotation, float angle)
{
	this->modelMatrix = glm::rotate(this->modelMatrix, angle, rotation);
}

// - Saber si es o no un plano
bool Element3D::elementIsPlane()
{
	return isPlane;
}

// - Asignar par�metros del contorno b�sico
void Element3D::setBasicOutline(BasicOutline basic)
{
	this->basicOutline = basic;
	initialBasicOutline = this->basicOutline;
}

// - Asignar par�metros del contorno b�sico
BasicOutline& Element3D::getBasicOutline()
{
	return this->basicOutline;
}

// - Restaurar contorno avanzado a su estado por defecto
void Element3D::resetBasicOutline()
{
	this->basicOutline.color = initialBasicOutline.color;
	this->basicOutline.thickness = initialBasicOutline.thickness;
}

// - Asignar par�metros del contorno avanzado
void Element3D::setAdvancedOutline(AdvancedOutline advanced)
{
	this->advancedOutline = advanced;
	initialAdvancedOutline = this->advancedOutline;
}

// - Asignar par�metros del contorno avanzado
AdvancedOutline& Element3D::getAdvancedOutline()
{
	return this->advancedOutline;
}

// - Restaurar contorno avanzado a su estado por defecto
void Element3D::resetAdvancedOutline()
{
	this->advancedOutline.color = initialAdvancedOutline.color;
	this->advancedOutline.thickness = initialAdvancedOutline.thickness;
	this->advancedOutline.extension = initialAdvancedOutline.extension;
}

// - Asignar par�metros de la t�cnica Monochrome
void Element3D::setMonochromeTechnique(MonochromeTechnique monochrome)
{
	this->monochrome = monochrome;
	initialMonochrome = this->monochrome;
}

// - Obtener par�metros de la t�cnica Monochrome
MonochromeTechnique& Element3D::getMonochromeTechnique()
{
	return this->monochrome;
}

// - Restaurar t�cnica Monochrome a su estado por defecto
void Element3D::resetMonochromeTechnique()
{
	this->monochrome = initialMonochrome;
}

// - Asignar par�metros de la t�cnica Cel-Shading
void Element3D::setCelShadingTechnique(CelShadingTechnique celShading)
{
	this->celShading = celShading;
	initialCelShading = this->celShading;
}

// - Obtener par�metros de la t�cnica Cel-Shading
CelShadingTechnique& Element3D::getCelShadingTechnique()
{
	return this->celShading;
}

// - Restaurar t�cnica Cel-Shading a su estado por defecto
void Element3D::resetCelShadingTechnique()
{
	this->celShading = initialCelShading;
}

// - Asignar par�metros de la t�cnica Hatching
void Element3D::setHatchingTechnique(HatchingTechnique hatching)
{
	this->hatching = hatching;
	initialHatching = this->hatching;
}

// - Obtener par�metros de la t�cnica Hatching
HatchingTechnique& Element3D::getHatchingTechnique()
{
	return this->hatching;
}

// - Restaurar t�cnica Hatching a su estado por defecto
void Element3D::resetHatchingTechnique()
{
	this->hatching = initialHatching;
}

// - Asignar par�metros de la t�cnica Gooch Shading
void Element3D::setGoochShadingTechnique(GoochShadingTechnique goochShading)
{
	this->goochShading = goochShading;
	initialGoochShading = this->goochShading;
}

// - Obtener par�metros de la t�cnica Gooch Shading
GoochShadingTechnique& Element3D::getGoochShadingTechnique()
{
	return this->goochShading;
}

// - Restaurar t�cnica Gooch Shading a su estado por defecto
void Element3D::resetGoochShadingTechnique()
{
	this->goochShading = initialGoochShading;
}