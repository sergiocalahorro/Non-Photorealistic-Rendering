#include "LightSource.h"

// - Constructor
LightSource::LightSource()
{

}

// - Destructor
LightSource::~LightSource()
{

}

// - Aplicar el shader de la fuente luminosa
void LightSource::apply(ShaderProgram &shader)
{
	if (applicator != nullptr)
	{
		applicator->apply(shader, this);
	}
}

// - Obtener tipo de aplicador
LightApplicator* LightSource::getApplicator()
{
	return applicator;
}

// - Obtener el tipo de fuente luminosa
std::string LightSource::getType()
{
	return type;
}

// - Obtener intensidad de la luz ambiental
glm::vec3& LightSource::getIa()
{
	return Ia;
}

// - Obtener intensidad de la luz difusa
glm::vec3& LightSource::getId()
{
	return Id;
}

// - Obtener intensidad de la luz especular
glm::vec3& LightSource::getIs()
{
	return Is;
}

// - Obtener posición de la luz (Point light)
glm::vec3& LightSource::getPosition()
{
	return position;
}

// - Obtener dirección de la luz (Directional y spot light)
glm::vec3& LightSource::getDirection()
{
	return direction;
}

// - Obtener ángulo (Spot light)
float& LightSource::getAngle()
{
	return angle;
}

// - Obtener exponente (Spot light)
float& LightSource::getExponent()
{
	return exponent;
}

// - Asignar tipo de aplicador
void LightSource::setApplicator(LightApplicator* applicator)
{
	this->applicator = applicator;
}

// - Asignar el tipo de fuente luminosa
void LightSource::setType(std::string type)
{
	this->type = type;
}

// - Asignar intensidad de la luz ambiental
void LightSource::setIa(glm::vec3 Ia)
{
	this->Ia = Ia;
}

// - Asignar intensidad de la luz difusa
void LightSource::setId(glm::vec3 Id)
{
	this->Id = Id;
}

// - Asignar intensidad de la luz especular
void LightSource::setIs(glm::vec3 Is)
{
	this->Is = Is;
}

// - Asignar posición de la luz (Point light)
void LightSource::setPosition(glm::vec3 position)
{
	this->position = position;
}

// - Asignar dirección de la luz (Directional y spot light)
void LightSource::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

// - Asignar ángulo (Spot light)
void LightSource::setAngle(float angle)
{
	this->angle = angle;
}

// - Asignar exponente (Spot light)
void LightSource::setExponent(float exponent)
{
	this->exponent = exponent;
}

// - Obtener estado de activación de la luz
bool& LightSource::isLightEnabled()
{
	return enabled;
}

// - Obtener estado de activación de la luz
void LightSource::setLightStatus(bool status)
{
	enabled = status;
}

// - Reiniciar tipo de fuente
void LightSource::setInitialStatus()
{
	if (type == "Ambient")
	{
		initialIa = Ia;
	}

	if (type == "Point")
	{
		initialId = Id;
		initialIs = Is;
		initialPosition = position;
	}

	if (type == "Directional")
	{
		initialId = Id;
		initialIs = Is;
		initialDirection = direction;
	}

	if (type == "Spot")
	{
		initialId = Id;
		initialIs = Is;
		initialPosition = position;
		initialDirection = direction;
		initialAngle = angle;
		initialExponent = exponent;
	}
}

// - Reiniciar tipo de fuente
void LightSource::resetInitialStatus()
{
	if (type == "Ambient")
	{
		Ia = initialIa;
	}

	if (type == "Point")
	{
		Id = initialId;
		Is = initialIs;
		position = initialPosition;
	}

	if (type == "Directional")
	{
		Id = initialId;
		Is = initialIs;
		direction = initialDirection;
	}

	if (type == "Spot")
	{
		Id = initialId;
		Is = initialIs;
		position = initialPosition;
		direction = initialDirection;
		angle = initialAngle;
		exponent = initialExponent;
	}
}