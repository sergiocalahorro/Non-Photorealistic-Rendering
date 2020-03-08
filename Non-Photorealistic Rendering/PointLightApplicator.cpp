#include "PointLightApplicator.h"

// - Constructor
PointLightApplicator::PointLightApplicator()
{

}

// - Destructor
PointLightApplicator::~PointLightApplicator()
{

}

// - Aplicar luz direccional
void PointLightApplicator::apply(ShaderProgram &shader, LightSource *light)
{
	// - Asignar uniforms
	shader.setUniform("Id", light->getId());
	shader.setUniform("Is", light->getIs());
	shader.setUniform("lightPosition", light->getPosition());

	// - Constantes de atenuación de la fuente luminosa
	shader.setUniform("constant", 1.f);
	shader.setUniform("linear", 0.f);
	shader.setUniform("quadratic", 0.f);

	// - Utilizar subrutina
	shader.setSubroutine("LightUniform", "PointLight", GL_FRAGMENT_SHADER, 1);
}