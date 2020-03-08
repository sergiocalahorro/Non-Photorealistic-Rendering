#pragma once

#include "LightApplicator.h"

class PointLightApplicator: public LightApplicator
{
public:
	// - Constructor
	PointLightApplicator();

	// - Destructor
	~PointLightApplicator();

	// - Aplicar luz puntual
	void apply(ShaderProgram &shader, LightSource *light) override;
};