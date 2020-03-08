#pragma once

#include <glm.hpp>
#include <iostream>

class Camera
{
private:
	// - Par�metros para calcular la matriz de visi�n
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;
	glm::mat4 viewMatrix;

	// - Par�metros para calcular la matriz de proyecci�n
	float aspect;
	float fovX;
	float fovY;
	float zNear;
	float zFar;
	glm::mat4 projectionMatrix;

	// - Variables para almacenar lospar�metros iniciales de la c�mara (para resetear)
	glm::vec3 initialPosition;
	glm::vec3 initialLookAt;
	float initialAspect;
	float initialFOVX;
	float initialFOVY;
	float initialZNear;
	float initialZFar;

	// - Matrices de visi�n y proyecci�n iniciales (para resetear c�mara)
	glm::mat4 initialViewMatrix;
	glm::mat4 initialProjectionMatrix;

	// - L�mites de movimiento de la c�mara
	glm::vec3 minLimitPosition;
	glm::vec3 maxLimitPosition;

	glm::vec3 minLimitLookAt;
	glm::vec3 maxLimitLookAt;

	// - L�mites de campo de visi�n horizontal
	float minLimitFOVX;
	float maxLimitFOVX;

public:
	// - Constructor (con vectores)
	Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, 
		   int width, int height, float fovX, float zNear, float zFar, 
		   glm::vec3 minLimitPosition, glm::vec3 maxLimitPosition,
		   glm::vec3 minLimitLookAt, glm::vec3 maxLimitLookAt,
		   float minLimitFOVX, float maxLimitFOVX);

	// - Constructor (con escalares)
	Camera(float positionX, float positionY, float positionZ,
		   float lookAtX, float lookAtY, float lookAtZ,
	       float upX, float upY, float upZ,
		   int width, int height, float fovX, float zNear, float zFar, 
		   float minLimitPositionX, float minLimitPositionY, float minLimitPositionZ,
		   float maxLimitPositionX, float maxLimitPositionY, float maxLimitPositionZ,
		   float minLimitLookAtX, float minLimitLookAtY, float minLimitLookAtZ,
		   float maxLimitLookAtX, float maxLimitLookAtY, float maxLimitLookAtZ,
		   float minLimitFOVX, float maxLimitFOVX);

	// - Destructor
	~Camera();

	// - C�lculo de matrices
	void calculateViewMatrix(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);
	void calculateViewMatrix();

	void calculateProjectionMatrix();
	void calculateProjectionMatrix(int width, int height, float fovX,
								   float zNear, float zFar);

	// - Asignar par�metros de la c�mara iniciales
	void setInitialStatus();

	// - Resetear par�metros de la c�mara a su estado inicial
	void reset();

	// - Comprobar l�mites de movimiento (posici�n y lookAt) en ejes
	void checkLimits();

	// - Movimientos de c�mara
	void pan(float angle);
	void tilt(float angle);
	void truck(float movX);
	void boom(float movY);
	void dolly(float movZ);
	void orbit(float angle);
	void zoom(float factor);

	// - Getters
	glm::vec3& getPosition();
	glm::vec3& getLookAt();
	float& getFovX();
	float& getZNear();
	float& getZFar();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewProjectionMatrix();

	glm::vec3& getMinLimitPosition();
	glm::vec3& getMaxLimitPosition();
	glm::vec3& getMinLimitLookAt();
	glm::vec3& getMaxLimitLookAt();
	float& getMinLimitFOVX();
	float& getMaxLimitFOVX();

	// - Setters
	void setPosition(glm::vec3 position);
	void setAspect(unsigned int width, unsigned int height);
};