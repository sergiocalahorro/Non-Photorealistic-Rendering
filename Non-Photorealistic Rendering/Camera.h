#pragma once

#include <glm.hpp>
#include <iostream>

class Camera
{
private:
	// - Parámetros para calcular la matriz de visión
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;
	glm::mat4 viewMatrix;

	// - Parámetros para calcular la matriz de proyección
	float aspect;
	float fovX;
	float fovY;
	float zNear;
	float zFar;
	glm::mat4 projectionMatrix;

	// - Variables para almacenar losparámetros iniciales de la cámara (para resetear)
	glm::vec3 initialPosition;
	glm::vec3 initialLookAt;
	float initialAspect;
	float initialFOVX;
	float initialFOVY;
	float initialZNear;
	float initialZFar;

	// - Matrices de visión y proyección iniciales (para resetear cámara)
	glm::mat4 initialViewMatrix;
	glm::mat4 initialProjectionMatrix;

	// - Límites de movimiento de la cámara
	glm::vec3 minLimitPosition;
	glm::vec3 maxLimitPosition;

	glm::vec3 minLimitLookAt;
	glm::vec3 maxLimitLookAt;

	// - Límites de campo de visión horizontal
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

	// - Cálculo de matrices
	void calculateViewMatrix(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);
	void calculateViewMatrix();

	void calculateProjectionMatrix();
	void calculateProjectionMatrix(int width, int height, float fovX,
								   float zNear, float zFar);

	// - Asignar parámetros de la cámara iniciales
	void setInitialStatus();

	// - Resetear parámetros de la cámara a su estado inicial
	void reset();

	// - Comprobar límites de movimiento (posición y lookAt) en ejes
	void checkLimits();

	// - Movimientos de cámara
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