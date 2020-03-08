#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/rotate_vector.hpp>

// - Constructor (con vectores)
Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, 
			   int width, int height, float fovX, float zNear, float zFar, 
			   glm::vec3 minLimitPosition, glm::vec3 maxLimitPosition,
			   glm::vec3 minLimitLookAt, glm::vec3 maxLimitLookAt,
			   float minLimitFOVX, float maxLimitFOVX)
{
	// - Matriz de visi�n
	calculateViewMatrix(position, lookAt, up);

	// - Matriz de proyecci�n
	calculateProjectionMatrix(width, height, fovX, zNear, zFar);
	
	// - L�mites de movimiento de la c�mara
	this->minLimitPosition = minLimitPosition;
	this->maxLimitPosition = maxLimitPosition;

	this->minLimitLookAt = minLimitLookAt;
	this->maxLimitLookAt = maxLimitLookAt;

	// - L�mites de campo de visi�n horizontal
	this->minLimitFOVX = minLimitFOVX;
	this->maxLimitFOVX = maxLimitFOVX;
}

// - Constructor (con escalares)
Camera::Camera(float positionX, float positionY, float positionZ, 
			   float lookAtX, float lookAtY, float lookAtZ, 
			   float upX, float upY, float upZ, 
			   int width, int height, float fovX, float zNear, float zFar,
			   float minLimitPositionX, float minLimitPositionY, float minLimitPositionZ,
			   float maxLimitPositionX, float maxLimitPositionY, float maxLimitPositionZ,
			   float minLimitLookAtX, float minLimitLookAtY, float minLimitLookAtZ,
			   float maxLimitLookAtX, float maxLimitLookAtY, float maxLimitLookAtZ,
			   float minLimitFOVX, float maxLimitFOVX)
{
	// - Matriz de visi�n
	calculateViewMatrix(glm::vec3(positionX, positionY, positionZ),
						glm::vec3(lookAtX, lookAtY, lookAtZ),
						glm::vec3(upX, upY, upZ));

	// - Matriz de proyecci�n
	calculateProjectionMatrix(width, height, fovX, zNear, zFar);

	// - L�mites de movimiento de la c�mara
	minLimitPosition = glm::vec3(minLimitPositionX, minLimitPositionY, minLimitPositionZ);
	maxLimitPosition = glm::vec3(maxLimitPositionX, maxLimitPositionY, maxLimitPositionZ);

	minLimitLookAt = glm::vec3(minLimitLookAtX, minLimitLookAtY, minLimitLookAtZ);
	maxLimitLookAt = glm::vec3(maxLimitLookAtX, maxLimitLookAtY, maxLimitLookAtZ);

	// - L�mites de campo de visi�n horizontal
	this->minLimitFOVX = minLimitFOVX;
	this->maxLimitFOVX = maxLimitFOVX;
}

// - Destructor
Camera::~Camera()
{

}

// - Calcular matriz de visi�n
void Camera::calculateViewMatrix(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
{
	this->position = position;
	this->lookAt = lookAt;
	this->up = up;

	viewMatrix = glm::lookAt(this->position, this->lookAt, this->up);
}

// - Calcular matriz de proyecci�n
void Camera::calculateProjectionMatrix(int width, int height, float fovX, 
									   float zNear, float zFar)
{
	this->aspect = float (width) / height;
	this->fovX = fovX;
	this->fovY = 2 * glm::atan(glm::tan(glm::radians(this->fovX) / 2) / this->aspect);
	this->zNear = zNear;
	this->zFar = zFar;

	projectionMatrix = glm::perspective(this->fovY, this->aspect, this->zNear, this->zFar);
}

// - Calcular matriz de visi�n
void Camera::calculateViewMatrix()
{
	viewMatrix = glm::lookAt(this->position, this->lookAt, this->up);
}

// - Calcular matriz de proyecci�n
void Camera::calculateProjectionMatrix()
{
	this->fovY = 2 * glm::atan(glm::tan(glm::radians(this->fovX) / 2) / this->aspect);
	projectionMatrix = glm::perspective(this->fovY, this->aspect, this->zNear, this->zFar);
}

// - Asignar par�metros de la c�mara iniciales
void Camera::setInitialStatus()
{
	initialPosition = position;
	initialLookAt = lookAt;
	initialFOVX = fovX;
	initialFOVY = fovY;
	initialAspect = aspect;
	initialZNear = zNear;
	initialZFar = zFar;
	initialViewMatrix = viewMatrix;
	initialProjectionMatrix = projectionMatrix;
}

// - Resetear par�metros variables de la c�mara a su estado inicial
void Camera::reset()
{
	position = initialPosition;
	lookAt = initialLookAt;
	fovX = initialFOVX;
	fovY = initialFOVY;
	aspect = initialAspect;
	zNear = initialZNear;
	zFar = initialZFar;
	viewMatrix = initialViewMatrix;
	projectionMatrix = initialProjectionMatrix;
}

// - Comprobar l�mites de movimiento (posici�n y lookAt) en ejes
void Camera::checkLimits()
{
	// - Limitar movimiento en eje X
	if (position.x <= minLimitPosition.x)
	{
		position.x = minLimitPosition.x;
	}
	else if (position.x >= maxLimitPosition.x)
	{
		position.x = maxLimitPosition.x;
	}

	if (lookAt.x <= minLimitLookAt.x)
	{
		lookAt.x = minLimitLookAt.x;
	}
	else if (lookAt.x >= maxLimitLookAt.x)
	{
		lookAt.x = maxLimitLookAt.x;
	}

	// - Limitar movimiento en eje Y
	if (position.y <= minLimitPosition.y)
	{
		position.y = minLimitPosition.y;
	}
	else if (position.y >= maxLimitPosition.y)
	{
		position.y = maxLimitPosition.y;
	}

	if (lookAt.y <= minLimitLookAt.y)
	{
		lookAt.y = minLimitLookAt.y;
	}
	else if (lookAt.y >= maxLimitLookAt.y)
	{
		lookAt.y = maxLimitLookAt.y;
	}

	// - Limitar movimiento en eje Z
	if (position.z <= minLimitPosition.z)
	{
		position.z = minLimitPosition.z;
	}
	else if (position.z >= maxLimitPosition.z)
	{
		position.z = maxLimitPosition.z;
	}

	if (lookAt.z <= minLimitLookAt.z)
	{
		lookAt.z = minLimitLookAt.z;
	}
	else if (lookAt.z >= maxLimitLookAt.z)
	{
		lookAt.z = maxLimitLookAt.z;
	}
}

// - Movimiento de c�mara: Pan
void Camera::pan(float angle)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = glm::normalize(lookAt - position);

	// - Calcular vector de direcci�n de visi�n	
	glm::vec3 normal = glm::cross(glm::cross(viewDirection, up), viewDirection);

	// - Calcular desplazamiento en la direcci�n del eje
	glm::vec3 shift = glm::rotate(lookAt - position, glm::radians(angle), normal);

	// - Aplicar desplazamiento en el punto de inter�s en funci�n de la posici�n
	lookAt = position + shift;

	// - Recalcular matriz de visi�n
	viewMatrix = glm::lookAt(position, lookAt, up);
}

// - Movimiento de c�mara: Tilt
void Camera::tilt(float angle)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = glm::normalize(lookAt - position);

	// - Calcular vector de direcci�n de visi�n	
	glm::vec3 normal = glm::cross(viewDirection, up);

	// - Calcular desplazamiento en la direcci�n del eje
	glm::vec3 shift = glm::rotate(lookAt - position, glm::radians(angle), normal);

	// - Aplicar desplazamiento en el punto de inter�s en funci�n de la posici�n
	lookAt = position + shift;

	// - Recalcular matriz de visi�n
	viewMatrix = glm::lookAt(position, lookAt, up);
}

// - Movimiento de c�mara: Truck
void Camera::truck(float movX)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = glm::normalize(lookAt - position);

	// - Calcular el eje del vector de movimiento
	glm::vec3 normal = glm::cross(viewDirection, up);

	// - Calcular desplazamiento en la direcci�n del eje
	glm::vec3 shift = normal * movX;

	// - Aplicar desplazamiento en la posici�n y punto de inter�s de la c�mara
	position += shift;
	lookAt += shift;

	// - Recalcular matriz de visi�n
	viewMatrix = glm::lookAt(position, lookAt, up);
}

// - Movimiento de c�mara: Boom/Crane
void Camera::boom(float movY)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = glm::normalize(lookAt - position);

	// - Calcular el eje del vector de movimiento
	glm::vec3 normal = glm::cross(glm::cross(viewDirection, up), viewDirection);
	
	// - Calcular desplazamiento en la direcci�n del eje
	glm::vec3 shift = normal * movY;

	// - Aplicar desplazamiento en la posici�n y punto de inter�s de la c�mara
	position += shift;
	lookAt += shift;

	// - Recalcular matriz de visi�n
	viewMatrix = glm::lookAt(position, lookAt, up);
}

// - Movimiento de c�mara: Dolly
void Camera::dolly(float movZ)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = glm::normalize(lookAt - position);

	// - Calcular desplazamiento en la direcci�n de visi�n
	glm::vec3 shift = viewDirection * movZ;

	// - Aplicar desplazamiento en la posici�n y punto de inter�s de la c�mara
	if (position != lookAt)
	{
		position += shift;
		lookAt += shift;
	}

	// - Recalcular matriz de visi�n
	viewMatrix = glm::lookAt(position, lookAt, up);
}

// - Movimiento de c�mara: Orbit
void Camera::orbit(float angle)
{
	// - Comprobar l�mites
	checkLimits();

	// - Calcular vector de direcci�n de visi�n
	glm::vec3 viewDirection = lookAt - position;
		
	// - Calcular vector de rotaci�n en eje Y aplicando el �ngulo
	glm::vec3 rotation = glm::rotateY(viewDirection, glm::radians(angle));

	// - Modificar la posici�n de la c�mara aplicando la rotaci�n sobre el punto de inter�s
	position = lookAt - rotation;
	
	// - Recalcular matriz de visi�n
	if (position.z != lookAt.z)
	{
		viewMatrix = glm::lookAt(position, lookAt, up);
	}
}

// - Movimiento de c�mara: Zoom
void Camera::zoom(float factor)
{
	// - Modificar campo de visi�n horizontal (en grados)
	if (fovX >= minLimitFOVX && fovX <= maxLimitFOVX)
	{
		fovX -= factor;
	}

	// - Limitar campo de visi�n horizontal
	if (fovX < minLimitFOVX)
	{
		fovX = minLimitFOVX;
	}
	else if (fovX > maxLimitFOVX)
	{
		fovX = maxLimitFOVX;
	}

	// - Calcular nuevo campo de visi�n vertical a partir de la variaci�n realizada en el campo
	//   de visi�n horizontal
	fovY = 2 * glm::atan(glm::tan(glm::radians(this->fovX) / 2) / this->aspect);

	// - Recalcular nueva matriz de proyecci�n
	projectionMatrix = glm::perspective(fovY, aspect, zNear, zFar);
}

// - Obtener posici�n de la c�mara
glm::vec3& Camera::getPosition()
{
	return position;
}

// - Obtener lookAt de la c�mara
glm::vec3& Camera::getLookAt()
{
	return lookAt;
}

// - Obtener campo de visi�n horizontal (FOVX)
float& Camera::getFovX()
{
	return fovX;
}

// - Obtener plano 'near'
float& Camera::getZNear()
{
	return zNear;
}

// - Obtener plano 'far'
float& Camera::getZFar()
{
	return zFar;
}

// - Obtener la matriz de visi�n
glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

// - Obtener la matriz de proyecci�n
glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

// - Obtener la matriz combinada de visi�n y proyecci�n
glm::mat4 Camera::getViewProjectionMatrix()
{
	return projectionMatrix * viewMatrix;
}

// - Obtener l�mite m�nimo de posici�n
glm::vec3& Camera::getMinLimitPosition()
{
	return minLimitPosition;
}

// - Obtener l�mite m�ximo de posici�n
glm::vec3& Camera::getMaxLimitPosition()
{
	return maxLimitPosition;
}

// - Obtener l�mite m�nimo de lookAt
glm::vec3& Camera::getMinLimitLookAt()
{
	return minLimitLookAt;
}

// - Obtener l�mite m�ximo de lookAt
glm::vec3& Camera::getMaxLimitLookAt()
{
	return maxLimitLookAt;
}

// - Obtener l�mite m�nimo de FOVX
float& Camera::getMinLimitFOVX()
{
	return minLimitFOVX;
}

// - Obtener l�mite m�ximo de FOVX
float& Camera::getMaxLimitFOVX()
{
	return maxLimitFOVX;
}

// - Asignar nueva posici�n a la c�mara y actualizar la matriz de visi�n
void Camera::setPosition(glm::vec3 position)
{
	// - Prohibir cambiar la posici�n si supera los l�mites establecidos
	if ((position.x >= minLimitPosition.x && position.x <= maxLimitPosition.x) &&
		(position.y >= minLimitPosition.y && position.x <= maxLimitPosition.y) &&
		(position.z >= minLimitPosition.z && position.x <= maxLimitPosition.z))
	{
		this->position = position;

		// - Recalcular matriz de visi�n
		viewMatrix = glm::lookAt(position, lookAt, up);
	}
	else
	{
		std::cout << "La posicion de la camara introducida esta fuera de los limites" << std::endl;
	}
}

// - Asignar el aspect ratio y actualizar la matriz de proyecci�n
void Camera::setAspect(unsigned int width, unsigned int height)
{
	if (height > 0)
	{
		aspect = (float) width / height;

		// - Recalcular matriz de proyecci�n
		projectionMatrix = glm::perspective(fovY, aspect, zNear, zFar);
	}
}