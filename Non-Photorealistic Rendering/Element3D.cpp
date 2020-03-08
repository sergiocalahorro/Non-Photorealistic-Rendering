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

// - Transformación geométrica: traslación
void Element3D::translate(glm::vec3 translation)
{
	this->modelMatrix = glm::translate(this->modelMatrix, translation);
}

// - Transformación geométrica: escalado
void Element3D::scale(glm::vec3 scale)
{
	this->modelMatrix = glm::scale(this->modelMatrix, scale);
}

// - Transformación geométrica: rotación
void Element3D::rotate(glm::vec3 rotation, float angle)
{
	this->modelMatrix = glm::rotate(this->modelMatrix, angle, rotation);
}

// - Saber si es o no un plano
bool Element3D::elementIsPlane()
{
	return isPlane;
}

// - Asignar parámetros del contorno básico
void Element3D::setBasicOutline(BasicOutline basic)
{
	this->basicOutline = basic;
	initialBasicOutline = this->basicOutline;
}

// - Asignar parámetros del contorno básico
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

// - Asignar parámetros del contorno avanzado
void Element3D::setAdvancedOutline(AdvancedOutline advanced)
{
	this->advancedOutline = advanced;
	initialAdvancedOutline = this->advancedOutline;
}

// - Asignar parámetros del contorno avanzado
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

// - Asignar parámetros de la técnica Monochrome
void Element3D::setMonochromeTechnique(MonochromeTechnique monochrome)
{
	this->monochrome = monochrome;
	initialMonochrome = this->monochrome;
}

// - Obtener parámetros de la técnica Monochrome
MonochromeTechnique& Element3D::getMonochromeTechnique()
{
	return this->monochrome;
}

// - Restaurar técnica Monochrome a su estado por defecto
void Element3D::resetMonochromeTechnique()
{
	this->monochrome = initialMonochrome;
}

// - Asignar parámetros de la técnica Cel-Shading
void Element3D::setCelShadingTechnique(CelShadingTechnique celShading)
{
	this->celShading = celShading;
	initialCelShading = this->celShading;
}

// - Obtener parámetros de la técnica Cel-Shading
CelShadingTechnique& Element3D::getCelShadingTechnique()
{
	return this->celShading;
}

// - Restaurar técnica Cel-Shading a su estado por defecto
void Element3D::resetCelShadingTechnique()
{
	this->celShading = initialCelShading;
}

// - Asignar parámetros de la técnica Hatching
void Element3D::setHatchingTechnique(HatchingTechnique hatching)
{
	this->hatching = hatching;
	initialHatching = this->hatching;
}

// - Obtener parámetros de la técnica Hatching
HatchingTechnique& Element3D::getHatchingTechnique()
{
	return this->hatching;
}

// - Restaurar técnica Hatching a su estado por defecto
void Element3D::resetHatchingTechnique()
{
	this->hatching = initialHatching;
}

// - Asignar parámetros de la técnica Gooch Shading
void Element3D::setGoochShadingTechnique(GoochShadingTechnique goochShading)
{
	this->goochShading = goochShading;
	initialGoochShading = this->goochShading;
}

// - Obtener parámetros de la técnica Gooch Shading
GoochShadingTechnique& Element3D::getGoochShadingTechnique()
{
	return this->goochShading;
}

// - Restaurar técnica Gooch Shading a su estado por defecto
void Element3D::resetGoochShadingTechnique()
{
	this->goochShading = initialGoochShading;
}