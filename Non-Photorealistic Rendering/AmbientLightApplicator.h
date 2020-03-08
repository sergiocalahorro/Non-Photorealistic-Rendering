#pragma once

#include "LightApplicator.h"

class AmbientLightApplicator: public LightApplicator
{
public:
	// - Constructor
	AmbientLightApplicator();

	// - Destructor
	~AmbientLightApplicator();

	// - Aplicar luz ambiente
	void apply(ShaderProgram &shader, LightSource *light) override;
};