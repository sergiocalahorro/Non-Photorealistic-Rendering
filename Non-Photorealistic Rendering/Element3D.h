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

	// - Contorno básico
	BasicOutline basicOutline;
	BasicOutline initialBasicOutline;

	// - Contorno avanzado
	AdvancedOutline advancedOutline;
	AdvancedOutline initialAdvancedOutline;

	// - Técnica Monochrome
	MonochromeTechnique monochrome;
	MonochromeTechnique initialMonochrome;

	// - Técnica Cel-Shading
	CelShadingTechnique celShading;
	CelShadingTechnique initialCelShading;

	// - Técnica Hatching
	HatchingTechnique hatching;
	HatchingTechnique initialHatching;

	// - Técnica Gooch Shading
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

	// - Transformaciones geométricas
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

	// - Configuración de contorno básico
	void setBasicOutline(BasicOutline basic);
	BasicOutline& getBasicOutline();
	void resetBasicOutline();

	// - Configuración de contorno avanzado
	void setAdvancedOutline(AdvancedOutline advanced);
	AdvancedOutline& getAdvancedOutline();
	void resetAdvancedOutline();

	// - Configuración de la técnica Monochrome
	void setMonochromeTechnique(MonochromeTechnique monochrome);
	MonochromeTechnique& getMonochromeTechnique();
	void resetMonochromeTechnique();

	// - Configuración de la técnica Cel-Shading
	void setCelShadingTechnique(CelShadingTechnique celShading);
	CelShadingTechnique& getCelShadingTechnique();
	void resetCelShadingTechnique();

	// - Configuración de la técnica Hatching
	void setHatchingTechnique(HatchingTechnique hatching);
	HatchingTechnique& getHatchingTechnique();
	void resetHatchingTechnique();

	// - Configuración de la técnica Gooch Shading
	void setGoochShadingTechnique(GoochShadingTechnique goochShading);
	GoochShadingTechnique& getGoochShadingTechnique();
	void resetGoochShadingTechnique();
};