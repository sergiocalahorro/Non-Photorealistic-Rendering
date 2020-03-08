#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
#include <GL/glew.h>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Plane.h"
#include "Group3D.h"
#include "LightSource.h"

#include "Model.h"
#include "Cubemap.h"
#include "Quad.h"

#include "FBO.h"

// - GUI
#include "imgui.h"

// - La clase Renderer encapsula toda la gestión del área de dibujo
//   OpenGL, eventos, etc. 
// - Se implementa como un singleton para que pueda ser llamada desde
//   las funciones callback del main.cpp
class Renderer
{
private:
	// - Este es el singleton, la única instancia de la clase PagRenderer
    //   que se tiene en la aplicación
	static Renderer* instance;

	// - En un singleton el constructor es privado. Esto impide que se puedan construir
	//   nuevos renderers aparte del singleton
	Renderer();

	// - Viewport (dimensiones)
	unsigned int viewportWidth;
	unsigned int viewportHeight;

	// - Escenas 3D
	Group3D *scene1;
	Group3D *scene2;
	Group3D *scene3;

	// - Escenas 3D (modelos)
	Model *spiderman;
	Model *ironMan;
	Model *hulk;
	Model *captainAmerica;

	Model *fruitBowl;
	Model *room;

	Model *island;
	Model *statue;
	Model *treasure;

	// - Escenas 3D (plano)
	Plane *plane;

	// - Escena 3D (seleccionada actualmente)
	Group3D *currentScene;

	// - Elemento 3D seleccionado actualmente (de la escena seleccionada)
	Element3D *currentElement;

	// - Skybox
	Cubemap *skybox;

	// - Escena (carga)
	bool loadedScene1;
	bool loadedScene2;
	bool loadedScene3;

	// - Iluminación
	std::vector<LightSource*> lights;
	int numberOfLightsEnabled;

	// - Iluminación (configuración)
	void setupLighting();

	// - Iluminación (reinicio a su estado inicial)
	void lightSourceReset(std::string type);

	// - Cámara
	Camera *camera;

	// - Cámara (matriz de modelado, visión y proyección)
	glm::mat4 mvp;

	// - Cámara (configuración)
	void setupCamera();

	// - Quad (rendering a textura)
	Quad *quad;

	// - Rendering: Shader programs (modelos 3D y skyboxes)
	ShaderProgram realisticShader;
	ShaderProgram realisticSkyboxShader;

	ShaderProgram monochromeShader;

	ShaderProgram celShadingShader;
	ShaderProgram celShadingSkyboxShader;

	ShaderProgram hatchingShader;
	ShaderProgram hatchingSkyboxShader;

	ShaderProgram goochShadingShader;

	// - Rendering: Técnicas (modelos 3D)
	void realistic();
	void monochrome();
	void celShading();
	void hatching();
	void goochShading();

	// - Rendering: Control de rendering de técnicas (modelos 3D)
	bool enabledRealistic;
	bool enabledMonochrome;
	bool enabledCelShading;
	bool enabledHatching;
	bool enabledGoochShading;

	// - Rendering: Shader programs (post-procesamiento)
	ShaderProgram halftoneShader;
	ShaderProgram ditheringShader;
	ShaderProgram pixelArtShader;
	ShaderProgram painterlyShader;
	ShaderProgram charcoalShader;

	// - Rendering: Técnicas (post-procesamiento)
	void halftone();
	void dithering();
	void pixelArt();
	void painterly();
	void charcoal();

	// - Rendering: Control de rendering de técnicas (post-procesamiento)
	bool enabledHalftone;
	bool enabledDithering;
	bool enabledPixelArt;
	bool enabledPainterly;
	bool enabledCharcoal;

	// - Rendering: Shader programs (contornos)
	ShaderProgram basicOutlineShader;
	ShaderProgram advancedOutlineShader;

	// - Rendering: Contornos
	void basicOutline();
	void advancedOutline();

	// - FBO (rendering a textura y captura de pantalla)
	FBO *fbo;
	FBO *fboScreenshot;

	// - GUI (control sobre la ventana principal)
	bool enabledMainWindowGUI;
	bool enabledScroll;

	// - GUI (indicador de técnica y escena seleccionadas)
	int selectedScene;
	int selectedTechnique;

	// - GUI (indicador de elemento de escena seleccionado)
	int selectedElementScene1;
	int selectedElementScene2;
	int selectedElementScene3;
	int selectedElementScene4;

	// - GUI (secciones)
	void scenesGUI();
	void cameraGUI();
	void lightingGUI();
	void renderingGUI();
	void miscellaneousGUI();

	// - Captura de pantalla (dimensiones, control, contador, nombre)
	unsigned int screenshotWidth;
	unsigned int screenshotHeight;
	bool screenshotTaken;
	unsigned int screenshotCounter;
	std::string screenshotName;

	// - Captura de pantalla (control sobre tipo de técnica utilizada, para utilizar tamaño de ventana)
	bool enabledPostProcessing;

public:
	// - Destructor
	~Renderer();

	// - Este método de clase permite acceder al singleton. Cada vez
    //   que se necesite llamar al renderer se hará a través de este
    //   método
	static Renderer* getInstance();

	// - Aquí se definen los métodos que gestionarán el área de dibujo
    //   OpenGL al responder a los distintos eventos
	void windowRefreshCallback();
	void frameBufferSizeCallback(int width, int height);
	void keyCallback();
	void mouseButtonCallback();
	void scrollCallback(double xoffset, double yoffset);

	// - Rendering (preparar escena)
	void prepareOpenGL(int width, int height);

	// - Rendering (renderizar escena)
	void render();

	// - Rendering (activar modo de rendering y desactivar todos los modos de rendering)
	void setRenderingMode(std::string mode);
	void resetAllRenderingModes();

	// - Preparar escena
	void setupScene(unsigned int scene);

	// - Contornos (Activar/desactivar dibujado)
	void toggleBasicOutline();
	void toggleAdvancedOutline();

	// - Cámara (asignar posición y aspect ratio)
	void setCameraPosition(glm::vec3 position);
	void setCameraAspect(int width, int height);

	// - Cámara (movimientos)
	void cameraPanMovement(float rotX);
	void cameraTiltMovement(float rotY);
	void cameraTruckMovement(float movX);
	void cameraBoomMovement(float movY);
	void cameraDollyMovement(float movZ);
	void cameraOrbitMovement(float angle);
	void cameraZoom(float yoffset);

	// - Cámara (reinicio a estado inicial)
	void cameraReset();

	// - GUI (renderizar)
	void renderMainGUI();

	// - GUI (activar/desactivar visibilidad de la GUI)
	void toggleWindow();

	// - GUI (permitir scroll con la rueda del ratón sobre la ventana)
	bool isScrollEnabled();

	// - Captura de pantalla (asignar dimensiones, capturar pantalla)
	void setScreenshotDimensions(unsigned int width, unsigned int height);
	void takeScreenshot();
};