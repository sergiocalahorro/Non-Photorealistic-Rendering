#pragma once

#include "LightApplicator.h"

class SpotLightApplicator: public LightApplicator
{
public:
	// - Constructor
	SpotLightApplicator();

	// - Destructor
	~SpotLightApplicator();

	// - Aplicar spot light
	void apply(ShaderProgram &shader, LightSource *light) override;
};