#include "DirectionalLightApplicator.h"

// - Constructor
DirectionalLightApplicator::DirectionalLightApplicator()
{

}

// - Destructor
DirectionalLightApplicator::~DirectionalLightApplicator()
{

}

// - Aplicar luz direccional
void DirectionalLightApplicator::apply(ShaderProgram &shader, LightSource *light)
{
	// - Asignar uniforms
	shader.setUniform("Id", light->getId());
	shader.setUniform("Is", light->getIs());
	shader.setUniform("lightDirection", glm::normalize(light->getDirection()));

	// - Utilizar subrutina
	shader.setSubroutine("LightUniform", "DirectionalLight", GL_FRAGMENT_SHADER, 1);
}