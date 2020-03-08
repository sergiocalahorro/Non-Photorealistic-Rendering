#include "Material.h"

// - Constructor por defecto
Material::Material()
{

}

// - Constructor
Material::Material(glm::vec3 Kd, glm::vec3 Ks, float shininess)
{
	this->Kd = Kd;
	this->Ks = Ks;
	this->shininess = shininess;
}

// - Constructor (con color ambiente)
Material::Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess)
{
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks;
	this->shininess = shininess;
}

// - Destructor
Material::~Material()
{

}

// - Obtener color ambiente del material
glm::vec3& Material::getKa()
{
	return Ka;
}

// - Obtener color difuso del material
glm::vec3& Material::getKd() 
{
	return Kd;
}

// - Obtener color especular del material
glm::vec3& Material::getKs()
{
	return Ks;
}

// - Obtener exponente de brillo especular del material
float& Material::getShininess()
{
	return shininess;
}

// - Asignar color ambiente del material
void Material::setKa(glm::vec3 Ka)
{
	this->Ka = Ka;
}

// - Asignar color difuso del material
void Material::setKd(glm::vec3 Kd)
{
	this->Kd = Kd;
}

// - Asignar color especular del material
void Material::setKs(glm::vec3 Ks)
{
	this->Ks = Ks;
}

// - Asignar exponente de brillo especular del material
void Material::setShininess(float shininess)
{
	this->shininess = shininess;
}