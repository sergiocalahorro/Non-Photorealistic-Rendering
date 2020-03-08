#include "SpotLightApplicator.h"

// - Constructor
SpotLightApplicator::SpotLightApplicator()
{

}

// - Destructor
SpotLightApplicator::~SpotLightApplicator()
{

}

// - Aplicar spot light
void SpotLightApplicator::apply(ShaderProgram &shader, LightSource *light)
{
	// - Asignar uniforms
	shader.setUniform("Id", light->getId());
	shader.setUniform("Is", light->getIs());
	shader.setUniform("lightPosition", light->getPosition());
	shader.setUniform("lightDirection", glm::normalize(light->getDirection()));
	shader.setUniform("spotExponent", light->getExponent());
	shader.setUniform("cosGamma", glm::cos(glm::radians(light->getAngle())));

	// - Constantes de atenuación de la fuente luminosa
	shader.setUniform("constant", 1.f);
	shader.setUniform("linear", 0.f);
	shader.setUniform("quadratic", 0.f);

	// - Utilizar subrutina
	shader.setSubroutine("LightUniform", "SpotLight", GL_FRAGMENT_SHADER, 1);
}