#pragma once

#include <glm.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/rotate_vector.hpp>

#include "ShaderProgram.h"
#include "LightApplicator.h"

class LightApplicator; // - Forward declaration

class LightSource
{
private:
	// - Aplicador de la fuente luminosa
	LightApplicator* applicator;

	// - Parámetros de las fuentes luminosas
	std::string type;
	glm::vec3 Ia;
	glm::vec3 Id;
	glm::vec3 Is;
	glm::vec3 position;
	glm::vec3 direction;
	float angle;
	float exponent;

	// - Valores iniciales
	glm::vec3 initialIa;
	glm::vec3 initialId;
	glm::vec3 initialIs;
	glm::vec3 initialPosition;
	glm::vec3 initialDirection;
	float initialAngle;
	float initialExponent;

	// - Control de activación
	bool enabled;

public:

	// - Constructor
	LightSource();

	// - Destructor
	~LightSource();

	// - Aplicar el shader de la fuente luminosa
	void apply(ShaderProgram &shader);

	// - Getters
	LightApplicator* getApplicator();
	std::string getType();
	glm::vec3& getIa();
	glm::vec3& getId();
	glm::vec3& getIs();
	glm::vec3& getPosition();
	glm::vec3& getDirection();
	float& getAngle();
	float& getExponent();

	// - Setters
	void setApplicator(LightApplicator* applicator);
	void setType(std::string type);
	void setIa(glm::vec3 Ia);
	void setId(glm::vec3 Id);
	void setIs(glm::vec3 Is);
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setAngle(float angle);
	void setExponent(float exponent);

	// - Control de activación
	bool& isLightEnabled();
	void setLightStatus(bool status);

	// - Inicializar valores iniciales
	void setInitialStatus();
	
	// - Reiniciar tipo de fuente
	void resetInitialStatus();
};