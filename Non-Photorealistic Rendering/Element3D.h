#pragma once

#include "Material.h"
#include "ShaderProgram.h"
#include "Structures.h"

#include "Texture.h"

#include <gtc/matrix_transform.hpp>

class Element3D
{
protected:
	// - Matriz de modelado
	glm::mat4 modelMatrix;

	// - Material
	Material *material;

	// - Flag para saber si es un plano o no
	bool isPlane;

	// - Contorno b�sico
	BasicOutline basicOutline;
	BasicOutline initialBasicOutline;

	// - Contorno avanzado
	AdvancedOutline advancedOutline;
	AdvancedOutline initialAdvancedOutline;

	// - T�cnica Monochrome
	MonochromeTechnique monochrome;
	MonochromeTechnique initialMonochrome;

	// - T�cnica Cel-Shading
	CelShadingTechnique celShading;
	CelShadingTechnique initialCelShading;

	// - T�cnica Hatching
	HatchingTechnique hatching;
	HatchingTechnique initialHatching;

	// - T�cnica Gooch Shading
	GoochShadingTechnique goochShading;
	GoochShadingTechnique initialGoochShading;

public:
	// - Constructor por defecto
	Element3D();

	// - Destructor
	virtual ~Element3D();

	// - Getters
	glm::mat4 getModelMatrix();
	bool elementIsPlane();

	// - Setters
	void setModelMatrix(glm::mat4 modelMatrix);

	// - Transformaciones geom�tricas
	void translate(glm::vec3 translation);
	void scale(glm::vec3 scale);
	void rotate(glm::vec3 rotation, float angle);

	// - Dibujado del elemento 3D de distintas formas
	virtual void drawRealistic(ShaderProgram &shader, glm::mat4 mModel,
							   glm::mat4 mView, glm::mat4 mProjection) = 0;

	virtual void drawMonochrome(ShaderProgram &shader, glm::mat4 mModel,
							    glm::mat4 mView, glm::mat4 mProjection) = 0;

	virtual void drawCelShading(ShaderProgram &shader, glm::mat4 mModel,
								glm::mat4 mView, glm::mat4 mProjection) = 0;

	virtual void drawHatching(ShaderProgram &shader, glm::mat4 mModel,
						      glm::mat4 mView, glm::mat4 mProjection) = 0;

	virtual void drawGoochShading(ShaderProgram &shader, glm::mat4 mModel,
								  glm::mat4 mView, glm::mat4 mProjection) = 0;

	// - Dibujar contornos
	virtual void drawBasicOutline(ShaderProgram &shader, glm::mat4 mModel,
							      glm::mat4 mView, glm::mat4 mProjection) = 0;
	 
	virtual void drawAdvancedOutline(ShaderProgram &shader, glm::mat4 mModel,
									 glm::mat4 mView, glm::mat4 mProjection) = 0;

	// - Configuraci�n de contorno b�sico
	void setBasicOutline(BasicOutline basic);
	BasicOutline& getBasicOutline();
	void resetBasicOutline();

	// - Configuraci�n de contorno avanzado
	void setAdvancedOutline(AdvancedOutline advanced);
	AdvancedOutline& getAdvancedOutline();
	void resetAdvancedOutline();

	// - Configuraci�n de la t�cnica Monochrome
	void setMonochromeTechnique(MonochromeTechnique monochrome);
	MonochromeTechnique& getMonochromeTechnique();
	void resetMonochromeTechnique();

	// - Configuraci�n de la t�cnica Cel-Shading
	void setCelShadingTechnique(CelShadingTechnique celShading);
	CelShadingTechnique& getCelShadingTechnique();
	void resetCelShadingTechnique();

	// - Configuraci�n de la t�cnica Hatching
	void setHatchingTechnique(HatchingTechnique hatching);
	HatchingTechnique& getHatchingTechnique();
	void resetHatchingTechnique();

	// - Configuraci�n de la t�cnica Gooch Shading
	void setGoochShadingTechnique(GoochShadingTechnique goochShading);
	GoochShadingTechnique& getGoochShadingTechnique();
	void resetGoochShadingTechnique();
};