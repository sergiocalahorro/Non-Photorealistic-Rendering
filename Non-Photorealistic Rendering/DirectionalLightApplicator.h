#pragma once

#include "LightApplicator.h"

class DirectionalLightApplicator: public LightApplicator
{
public:
	// - Constructor
	DirectionalLightApplicator();

	// - Destructor
	~DirectionalLightApplicator();

	// - Aplicar luz direccional
	void apply(ShaderProgram &shader, LightSource *light) override;
};