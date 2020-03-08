#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>

class FBO
{
private:
	GLuint fboHandle;

	GLuint renderTex;
	GLuint depthTex;

public:
	// - Constructor
	FBO();

	// - Destructor
	~FBO();

	// - Enlazar el Frame Buffer
	void bindFrameBuffer();

	// - Desenlazar el Frame Buffer
	void unbindFrameBuffer();

	// - Activar unidad de textura
	void activeTextureUnit(unsigned int unit);

	// - Crear textura de color
	void createRenderTexture(unsigned int width, unsigned int height);

	// - Enlazar textura de color del FBO
	void bindRenderTexture();

	// - Desenlazar unidad de textura de color
	void unbindRenderTexture();

	// - Asociar buffer de color al FBO
	void attachRenderTexture();
	
	// - Crear buffer de profundidad del FBO
	void createDepthBuffer(unsigned int width, unsigned int height);

	// - Enlazar buffer de profundidad del FBO
	void bindDepthBuffer();

	// - Desenlazar buffer de profundidad
	void unbindDepthBuffer();

	// - Asociar buffer de profundidad al FBO
	void attachDepthBuffer();

	// - Comprobar estado del FBO
	void checkStatus();

	// - Tomar captura de pantalla
	void takeScreenshot(std::string &filename, unsigned int width, unsigned int height);
};