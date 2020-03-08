#pragma once
#include <glm.hpp>
#include <string>

class Material
{
private:
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float shininess;

public:
	// - Constructor por defecto
	Material();

	// - Constructor
	Material(glm::vec3 Kd, glm::vec3 Ks, float shininess);

	// - Constructor (con color ambiente)
	Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess);

	// - Destructor
	~Material();

	// - Getters
	glm::vec3& getKa();
	glm::vec3& getKd();
	glm::vec3& getKs();
	float& getShininess();

	// - Setters
	void setKa(glm::vec3 Ka);
	void setKd(glm::vec3 Kd);
	void setKs(glm::vec3 Ks);
	void setShininess(float shininess);
};