#include "AmbientLightApplicator.h"

// - Constructor
AmbientLightApplicator::AmbientLightApplicator()
{

}

// - Destructor
AmbientLightApplicator::~AmbientLightApplicator()
{

}

// - Aplicar Ambient light
void AmbientLightApplicator::apply(ShaderProgram &shader, LightSource *light)
{
	// - Asignar uniforms
	shader.setUniform("Ia", light->getIa());

	// - Utilizar subrutina
	shader.setSubroutine("LightUniform", "AmbientLight", GL_FRAGMENT_SHADER, 1);
}