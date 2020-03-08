#pragma once

#include "LightSource.h"

class LightSource; // - Forward declaration

class LightApplicator
{
public:
	// - Aplicar tipo de luz
	virtual void apply(ShaderProgram &shader, LightSource *light) = 0;
};