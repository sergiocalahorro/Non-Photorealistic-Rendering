#include "Renderer.h"
#include "AmbientLightApplicator.h"
#include "PointLightApplicator.h"
#include "DirectionalLightApplicator.h"
#include "SpotLightApplicator.h"

// - Aquí se inicializa el singleton. Todavía no se construye el objeto
//   de la clase Renderer porque se usa inicialización perezosa (lazy initialization)
// - La primera vez que se consulte el singleton se inicializará
Renderer* Renderer::instance = nullptr;

// - Constructor
Renderer::Renderer()
{

}

// - Destructor
Renderer::~Renderer()
{
	delete scene1;
	delete currentScene;
	delete camera;
	delete fbo;
}

// - Acceder al singleton.
//   Si es la primera vez que se llama a este método de clase, entonces
//   instance es 'nullptr' y se construye. Es lo que se llama inicialización
//   perezosa. Sólo se inicializa cuando se necesita
Renderer* Renderer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Renderer();
	}

	return instance;
}

// - Método que se encarga de redibujar el área OpenGL. Será llamado desde el callback
//   void window_refresh_callback(GLFWwindow *window). Por ahora lo único que hace es
//   borrar el área de dibujo OpenGL con la orden glClear(). En próximas prácticas se
//   irá completando
void Renderer::windowRefreshCallback()
{
	std::cout << "Refresh callback called" << std::endl;

	// - Borrar los buffers (color y profundidad).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// - Método que se llama cada vez que se cambie el tamaño del área de dibujo OpenGL. 
//   Será llamado desde el callback void framebuffer_size_callback(GLFWwindow *window,
//   int width, int height)
void Renderer::frameBufferSizeCallback(int width, int height)
{
	std::cout << "Resize callback called" << std::endl;
	glViewport(0, 0, width, height);

	// - Almacenar dimensiones del viewport
	viewportWidth = width;
	viewportHeight = height;

	// - Reasignar aspect ratio de la cámara
	camera->setAspect(width, height);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

	// - FBO (Rendering a textura): Redimensionar buffers de color y profundidad
	if (fbo != nullptr)
	{
		fbo->bindRenderTexture();
		fbo->createRenderTexture(width, height);
		fbo->unbindRenderTexture();

		fbo->bindDepthBuffer();
		fbo->createDepthBuffer(width, height);
		fbo->unbindDepthBuffer();
	}
}

// - Método que se llama cada vez que se pulse una tecla sobre el área de dibujo OpenGL.
//   Será llamado desde el callback void key_callback(GLFWwindow *window, int key, 
//   int scancode, int action, int mods)
void Renderer::keyCallback()
{
	std::cout << "Key callback called" << std::endl;
}

// - Método que se llama cada vez que se pulse un botón del ratón sobre el área de dibujo
//   OpenGL. Será llamado desde el callback void mouse_button_callback(GLFWwindow *window,
//   int button, int action, int mods)
void Renderer::mouseButtonCallback()
{
	std::cout << "Mouse button callback called" << std::endl;
}

// - Método que se llama cada vez que se mueva la rueda del ratón sobre el área de dibujo
//   OpenGL. Será llamado desde el callback void scroll_callback(GLFWwindow *window, 
//   double xoffset, double yoffset)
void Renderer::scrollCallback(double xoffset, double yoffset)
{
	std::cout << "Movida la rueda del raton " << xoffset <<
				 " Unidades en horizontal y " << yoffset <<
				 " unidades en vertical" << std::endl;
}

// - Método para preparar la escena para su visualización:
//		.Activación de parámetros globales de OpenGL
//		.Creación del objeto de revolución
//		.Creación de shaders y asignación de valores a las variables que utilizan
//		.Creación de cámara virtual
void Renderer::prepareOpenGL(int width, int height)
{
	// - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
	//   3D construido
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// - Activación de parámetros globales de OpenGL

	// - Establecemos el color negro como el color con el que se borrará el frame buffer.
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	glEnable(GL_DEPTH_TEST);

	// - Acepta sólo los valores de profundidad entrantes que sean menores o iguales
	//   que el valor de profundidad guardado
	glDepthFunc(GL_LEQUAL);

	// - Activar reinicio de primitivas
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFFFFFF);

	// - Activar uso de múltiples muestras de fragmentos en el cálculo del color final
	//   de un pixel
	glEnable(GL_MULTISAMPLE);

	// - Almacenar dimensiones del viewport
	viewportWidth = width;
	viewportHeight = height;

	// - Número de escena seleccionada
	selectedScene = 0;
	loadedScene1 = false;
	loadedScene2 = false;
	loadedScene3 = false;

	// - Creación del Skybox
	skybox = new Cubemap();

	// - Creación del Quad (rendering a textura)
	quad = new Quad();

	// - Preparar la escena
	setupScene(0);

	// - Número de fuentes luminosas activas
	numberOfLightsEnabled = 0;

	// - Shader programs de modelos
	realisticShader.createShaderProgram("Shaders/realistic");
	realisticSkyboxShader.createShaderProgram("Shaders/realisticSkybox");

	monochromeShader.createShaderProgram("Shaders/monochrome");

	celShadingShader.createShaderProgram("Shaders/celShading");
	celShadingSkyboxShader.createShaderProgram("Shaders/celShadingSkybox");

	hatchingShader.createShaderProgram("Shaders/hatching");
	hatchingSkyboxShader.createShaderProgram("Shaders/hatchingSkybox");

	goochShadingShader.createShaderProgram("Shaders/goochShading");

	// - Shader programs de post-procesamiento
	halftoneShader.createShaderProgram("Shaders/halftone");
	ditheringShader.createShaderProgram("Shaders/dithering");
	pixelArtShader.createShaderProgram("Shaders/pixelArt");
	painterlyShader.createShaderProgram("Shaders/painterly");
	charcoalShader.createShaderProgram("Shaders/charcoal");

	// - Shader programs de dibujado de contornos
	basicOutlineShader.createShaderProgram("Shaders/basicOutline");
	advancedOutlineShader.createShaderProgram("Shaders/advancedOutline", GEOMETRY_SHADER);

	// - Inicialmente desactivar todas las formas de dibujado, menos la realista
	enabledRealistic = true;

	// - Inicialmente no hay activado ningún shader proram de post-procesamiento
	enabledPostProcessing = false;

	// - Crear FBO
	fbo = new FBO();

	// - Crear y enlazar buffers de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();
	fbo->createRenderTexture(viewportWidth, viewportHeight);

	fbo->bindDepthBuffer();
	fbo->createDepthBuffer(viewportWidth, viewportHeight);

	// - Desenlazar buffers de color y profundidad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Enlazar buffers de color y profundidad al FBO
	fbo->attachRenderTexture();
	fbo->attachDepthBuffer();

	// - Comprobar que el FBO se creó correctamente
	fbo->checkStatus();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - Crear FBO para capturas de pantalla
	fboScreenshot = new FBO();

	// - Inicializar contador de capturas de pantalla y flag para saber si se tomó la captura
	screenshotCounter = 0;
	screenshotTaken = false;
	screenshotName = "";

	// - GUI
	enabledMainWindowGUI = true;
	enabledScroll = false;
}

// - Configurar la escena escena
void Renderer::setupScene(unsigned int scene)
{
	selectedScene = scene;

	if (selectedScene == 0)
	{
		/* 
		 * ESCENA 1
		 */
		if (!loadedScene1)
		{
			// - Cargar escena

			// - Modelo: Iron Man
			ironMan = new Model("Models/iron-man-mark-4/ironman_mark4.obj");
			ironMan->translate(glm::vec3(-6.f, -0.005f, -2.f));
			ironMan->scale(glm::vec3(0.041f));
			ironMan->rotate(glm::vec3(0.f, 1.f, 0.f), 29.8f);

			// - Contornos
			ironMan->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.003f));
			ironMan->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.003f, 0.0001f));

			// - Rendering (Monochrome)
			ironMan->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																		 glm::vec3(1.0f),
																		 glm::vec3(1.0f),
																		 8.f)));

			// - Rendering (Cel-Shading)
			ironMan->setCelShadingTechnique(CelShadingTechnique(2.f, 0.3f));

			// - Rendering (Hatching)
			ironMan->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			ironMan->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			ironMan->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Captain America
			captainAmerica = new Model("Models/M-FF_iOS_HERO_Steven_Rogers_Captain_America_Age_Of_Ultron/M-FF_iOS_HERO_Steven_Rogers_Captain_America_Age_Of_Ultron.obj");
			captainAmerica->translate(glm::vec3(-2.5f, -0.075f, -1.9f));
			captainAmerica->scale(glm::vec3(2.4f, 2.4f, 2.4f));

			// - Contornos
			captainAmerica->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.003f));
			captainAmerica->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.003f, 0.0001f));

			// - Rendering (Monochrome)
			captainAmerica->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																				glm::vec3(1.0f),
																				glm::vec3(1.0f),
																				8.f)));

			// - Rendering (Cel-Shading)
			captainAmerica->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			captainAmerica->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			captainAmerica->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			captainAmerica->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Hulk
			hulk = new Model("Models/hulk/Hulk.obj");
			hulk->translate(glm::vec3(-2.f, -0.1f, -2.6f));
			hulk->scale(glm::vec3(9.f, 9.f, 9.f));

			// - Contornos
			hulk->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.0015f));
			hulk->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.003f, 0.0001f));

			// - Rendering (Monochrome)
			hulk->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																	  glm::vec3(1.0f),
																	  glm::vec3(1.0f),
																	  8.f)));

			// - Rendering (Cel-Shading)
			hulk->setCelShadingTechnique(CelShadingTechnique(2.f, 0.3f));

			// - Rendering (Hatching)
			hulk->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			hulk->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			hulk->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Spider-Man
			spiderman = new Model("Models/Spider-Man/M-CoC_iOS_HERO_Peter_Parker_Spider-Man_Stark_Enhanced.obj");
			spiderman->translate(glm::vec3(5.75f, 0.015f, -1.6f));
			spiderman->scale(glm::vec3(2.5f));

			// - Contornos
			spiderman->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.004f));
			spiderman->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.0045f, 0.0001f));

			// - Rendering (Monochrome)
			spiderman->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																		   glm::vec3(1.0f),
																		   glm::vec3(1.0f),
																		   8.f)));

			// - Rendering (Cel-Shading)
			spiderman->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			spiderman->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			spiderman->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			spiderman->setGoochShadingTechnique(GoochShadingTechnique());

			// - Plano (dimensiones: 200 x 200, tiling horizontal: 50, tiling vertical: 50)
			plane = new Plane(200, 200, 50, 50);

			// - Cargar textura del plano
			plane->loadTexture("Textures/avengers-logo-wallpapers-for-android-On-wallpaper-hd.png");

			// - Rendering (Monochrome)
			plane->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f, 0.f, 0.f),
																	   glm::vec3(1.0f, 0.f, 0.f),
																	   glm::vec3(0.0f),
																	   8.f)));

			// - Rendering (Cel-Shading)
			plane->setCelShadingTechnique(CelShadingTechnique(1.f, 0.f));

			// - Rendering (Hatching)
			plane->setHatchingTechnique(HatchingTechnique(16.f, 45.f, 0.f));
			plane->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			plane->setGoochShadingTechnique(GoochShadingTechnique());

			// - Creación de la escena
			scene1 = new Group3D();
			scene1->addElement(ironMan);
			scene1->addElement(captainAmerica);
			scene1->addElement(hulk);
			scene1->addElement(spiderman);
			scene1->addElement(plane);

			// - Escena cargada
			loadedScene1 = true;
		}

		// - Cargar texturas del Skybox
		std::vector<std::string> skyboxTextures;
		skyboxTextures.push_back("Skyboxes/NightCity/right.png");
		skyboxTextures.push_back("Skyboxes/NightCity/left.png");
		skyboxTextures.push_back("Skyboxes/NightCity/top.png");
		skyboxTextures.push_back("Skyboxes/NightCity/bottom.png");
		skyboxTextures.push_back("Skyboxes/NightCity/front.png");
		skyboxTextures.push_back("Skyboxes/NightCity/back.png");
		skybox->loadTextures(skyboxTextures);

		// - Rendering (Cel-Shading)
		skybox->setCelShadingTechnique(CelShadingTechnique(6.f));

		// - Rendering (Hatching)
		skybox->setHatchingTechnique(HatchingTechnique(2.f, 160.f, 45.f, 0.f));
		skybox->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

		// - Escena seleccionada
		currentScene = scene1;
		selectedElementScene1 = 0;
		currentElement = currentScene->getElement(selectedElementScene1);
	}
	else if (selectedScene == 1)
	{
		/*
		 * ESCENA 2
		 */
		if (!loadedScene2)
		{
			// - Cargar escena

			// - Modelo: Habitación
			room = new Model("Models/the-big-bang-theory-virtual-apartment/sketchfablh3d.obj");
			room->translate(glm::vec3(-80.f, -5.f, 47.f));
			room->scale(glm::vec3(5.f));

			// - Contornos
			room->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.01f));
			room->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.01f, 0.0001f));

			// - Rendering (Monochrome)
			room->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																	  glm::vec3(1.0f),
																	  glm::vec3(1.0f),
																	  8.f)));

			// - Rendering (Cel-Shading)
			room->setCelShadingTechnique(CelShadingTechnique(3.f, 0.1f));

			// - Rendering (Hatching)
			room->setHatchingTechnique(HatchingTechnique(3.f, 30.f, 0.f));
			room->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			room->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Bol de frutas
			fruitBowl = new Model("Models/cesta-completa/Cesta Completa.obj");
			fruitBowl->translate(glm::vec3(-3.5f, -1.1f, 3.8f));
			fruitBowl->scale(glm::vec3(0.05f));

			// - Contornos
			fruitBowl->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.0075f));
			fruitBowl->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.01f, 0.0001f));

			// - Rendering (Monochrome)
			fruitBowl->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																		   glm::vec3(1.0f),
																		   glm::vec3(1.0f),
																		   8.f)));

			// - Rendering (Cel-Shading)
			fruitBowl->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			fruitBowl->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			fruitBowl->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			fruitBowl->setGoochShadingTechnique(GoochShadingTechnique());

			// - Creación de la escena
			scene2 = new Group3D();
			scene2->addElement(room);
			scene2->addElement(fruitBowl);

			// - Escena cargada
			loadedScene2 = true;
		}

		// - Cargar texturas del Skybox
		std::vector<std::string> skyboxTextures;
		skyboxTextures.push_back("Skyboxes/Parliament/right.png");
		skyboxTextures.push_back("Skyboxes/Parliament/left.png");
		skyboxTextures.push_back("Skyboxes/Parliament/top.png");
		skyboxTextures.push_back("Skyboxes/Parliament/bottom.png");
		skyboxTextures.push_back("Skyboxes/Parliament/front.png");
		skyboxTextures.push_back("Skyboxes/Parliament/back.png");
		skybox->loadTextures(skyboxTextures);

		// - Rendering (Cel-Shading)
		skybox->setCelShadingTechnique(CelShadingTechnique(6.f));

		// - Rendering (Hatching)
		skybox->setHatchingTechnique(HatchingTechnique(2.f, 160.f, 45.f, 0.f));
		skybox->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

		// - Escena seleccionada
		currentScene = scene2;
		selectedElementScene2 = 0;
		currentElement = currentScene->getElement(selectedElementScene2);
	}
	else if (selectedScene == 2)
	{
		/*
		 * ESCENA 3
		 */
		if (!loadedScene3)
		{
			// - Cargar escena

			// - Modelo: Isla
			island = new Model("Models/palm_island_OBJ/palm_island_OBJ.obj");
			island->scale(glm::vec3(0.015f));

			// - Contornos
			island->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.003f));
			island->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.005f, 0.0001f));

			// - Rendering (Monochrome)
			island->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																	    glm::vec3(1.0f),
																	    glm::vec3(1.0f),
																	    8.f)));

			// - Rendering (Cel-Shading)
			island->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			island->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			island->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			island->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Tesoro
			treasure = new Model("Models/TreasureChest_v2_L3.123c5b4249fc-a18b-4453-afc1-661a6b421f86/10803_TreasureChest_v2_L3.obj");
			treasure->translate(glm::vec3(-10.f, 0.25f, 5.f));
			treasure->scale(glm::vec3(0.03f));
			treasure->rotate(glm::vec3(1.f, 0.f, 0.f), 30.f);

			// - Contornos
			treasure->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.005f));
			treasure->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.005f, 0.0001f));

			// - Rendering (Monochrome)
			treasure->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																		  glm::vec3(1.0f),
																		  glm::vec3(1.0f),
																		  8.f)));

			// - Rendering (Cel-Shading)
			treasure->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			treasure->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			treasure->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			treasure->setGoochShadingTechnique(GoochShadingTechnique());

			// - Modelo: Estatua
			statue = new Model("Models/Statue_v1_L3.123cba36a8bf-c511-4678-98d4-db0007912819/12329_Statue_v1_l3.obj");
			statue->translate(glm::vec3(25.f, -2.7f, -10.f));
			statue->scale(glm::vec3(0.04f));
			statue->rotate(glm::vec3(1.f, 0.f, 0.f), 30.f);

			// - Contornos
			statue->setBasicOutline(BasicOutline(glm::vec3(0.f), 0.02f));
			statue->setAdvancedOutline(AdvancedOutline(glm::vec3(0.f), 0.02f, 0.0001f));

			// - Rendering (Monochrome)
			statue->setMonochromeTechnique(MonochromeTechnique(Material(glm::vec3(1.0f),
																		glm::vec3(1.0f),
																		glm::vec3(1.0f),
																		8.f)));

			// - Rendering (Cel-Shading)
			statue->setCelShadingTechnique(CelShadingTechnique(3.f, 0.3f));

			// - Rendering (Hatching)
			statue->setHatchingTechnique(HatchingTechnique(6.f, 0.f, 0.f));
			statue->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

			// - Rendering (Gooch Shading)
			statue->setGoochShadingTechnique(GoochShadingTechnique());

			// - Crear escena
			scene3 = new Group3D();
			scene3->addElement(island);
			scene3->addElement(treasure);
			scene3->addElement(statue);

			// - Escena cargada
			loadedScene3 = true;
		}

		// - Cargar texturas del Skybox
		std::vector<std::string> skyboxTextures;
		skyboxTextures.push_back("Skyboxes/SeaMountains/right.png");
		skyboxTextures.push_back("Skyboxes/SeaMountains/left.png");
		skyboxTextures.push_back("Skyboxes/SeaMountains/top.png");
		skyboxTextures.push_back("Skyboxes/SeaMountains/bottom.png");
		skyboxTextures.push_back("Skyboxes/SeaMountains/front.png");
		skyboxTextures.push_back("Skyboxes/SeaMountains/back.png");
		skybox->loadTextures(skyboxTextures);

		// - Rendering (Cel-Shading)
		skybox->setCelShadingTechnique(CelShadingTechnique(6.f));

		// - Rendering (Hatching)
		skybox->setHatchingTechnique(HatchingTechnique(2.f, 160.f, 45.f, 0.f));
		skybox->setHatchingTextures("Textures/hatch_dark.png", "Textures/hatch_bright.png");

		// - Escena seleccionada
		currentScene = scene3;
		selectedElementScene3 = 0;
		currentElement = currentScene->getElement(selectedElementScene3);
	}

	// - Preparar la cámara virtual
	setupCamera();

	// - Preparar fuentes luminosas
	setupLighting();
}

// - Método privado: Configurar la cámara virtual
void Renderer::setupCamera()
{
	if (selectedScene == 0)
	{
		/*
		 * ESCENA 1
		 */

		// - Parámetros para crear la matriz de visión
		glm::vec3 position(0.f, 3.f, 15.f);
		glm::vec3 lookAt(0.f, 3.f, -2.f);
		glm::vec3 up(0.f, 1.f, 0.f);

		// - Parámetros para crear la matriz de proyección
		float fovX = 50.f;
		float zNear = 0.1f;
		float zFar = 100000.f;

		// - Límites de movimiento de la cámara
		glm::vec3 minLimitPosition(-40.f, -12.f, -40.f);
		glm::vec3 maxLimitPosition(40.f, 12.f, 40.f);

		glm::vec3 minLimitLookAt(-39.f, -11.f, -39.f);
		glm::vec3 maxLimitLookAt(39.f, 11.f, 39.f);

		// - Límites de campo de visión horizontal
		float minFOVX = 10.f;
		float maxFOVX = 120.f;

		if (camera != nullptr)
		{
			delete camera;
		}

		// - Creación de la cámara
		camera = new Camera(position, lookAt, up,
							viewportWidth, viewportHeight,
							fovX, zNear, zFar,
							minLimitPosition, maxLimitPosition,
							minLimitLookAt, maxLimitLookAt,
							minFOVX, maxFOVX);

		// - Calcular matriz MVP (modelado, visión y proyección)
		mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

		// - Almacenar valores iniciales de la cámara
		camera->setInitialStatus();
	}
	else if (selectedScene == 1)
	{
		/*
		 * ESCENA 2
		 */

		// - Parámetros para crear la matriz de visión
		glm::vec3 position(-4.f, 2.f, 15.f);
		glm::vec3 lookAt(-4.f, 0.f, 2.5f);
		glm::vec3 up(0.f, 1.f, 0.f);

		// - Parámetros para crear la matriz de proyección
		float fovX = 100.f;
		float zNear = 0.1f;
		float zFar = 100000.f;

		// - Límites de movimiento de la cámara
		glm::vec3 minLimitPosition(-40.f, -20.f, -40.f);
		glm::vec3 maxLimitPosition(40.f, 20.f, 40.f);

		glm::vec3 minLimitLookAt(-39.f, -19.f, -39.f);
		glm::vec3 maxLimitLookAt(39.f, 19.f, 39.f);

		// - Límites de campo de visión horizontal
		float minFOVX = 10.f;
		float maxFOVX = 120.f;

		if (camera != nullptr)
		{
			delete camera;
		}

		// - Creación de la cámara
		camera = new Camera(position, lookAt, up,
							viewportWidth, viewportHeight,
							fovX, zNear, zFar,
							minLimitPosition, maxLimitPosition,
							minLimitLookAt, maxLimitLookAt,
							minFOVX, maxFOVX);

		// - Calcular matriz MVP (modelado, visión y proyección)
		mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

		// - Almacenar valores iniciales de la cámara
		camera->setInitialStatus();
	}
	else if (selectedScene == 2)
	{
		/*
		 * ESCENA 3
		 */

		// - Parámetros para crear la matriz de visión
		glm::vec3 position(-0.5f, 6.f, 28.f);
		glm::vec3 lookAt(-0.5f, 6.f, -4.5);
		glm::vec3 up(0.f, 1.f, 0.f);

		// - Parámetros para crear la matriz de proyección
		float fovX = 80.f;
		float zNear = 0.1f;
		float zFar = 100000.f;

		// - Límites de movimiento de la cámara
		glm::vec3 minLimitPosition(-40.f, -20.f, -40.f);
		glm::vec3 maxLimitPosition(40.f, 20.f, 40.f);

		glm::vec3 minLimitLookAt(-39.f, -19.f, -39.f);
		glm::vec3 maxLimitLookAt(39.f, 19.f, 39.f);

		// - Límites de campo de visión horizontal
		float minFOVX = 10.f;
		float maxFOVX = 120.f;

		if (camera != nullptr)
		{
		 delete camera;
		}

		// - Creación de la cámara
		camera = new Camera(position, lookAt, up,
							viewportWidth, viewportHeight,
							fovX, zNear, zFar,
							minLimitPosition, maxLimitPosition,
							minLimitLookAt, maxLimitLookAt,
							minFOVX, maxFOVX);

		// - Calcular matriz MVP (modelado, visión y proyección)
		mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

		// - Almacenar valores iniciales de la cámara
		camera->setInitialStatus();
	}
}

// - Método privado: Configurar fuentes luminosas
void Renderer::setupLighting()
{
	if (selectedScene == 0)
	{
		/*
		 * ESCENA 1
		 */

		// - Fuentes luminosas
		LightSource *ambientLight = new LightSource();
		LightSource *pointLight = new LightSource();
		LightSource *directionalLight = new LightSource();
		LightSource *spotLight = new LightSource();

		// - Fuente luminosa ambiente
		ambientLight->setApplicator(new AmbientLightApplicator());
		ambientLight->setType("Ambient");
		ambientLight->setIa(glm::vec3(0.25f, 0.25f, 0.25f));
		ambientLight->setLightStatus(true);

		// - Fuente luminosa puntual
		pointLight->setApplicator(new PointLightApplicator());
		pointLight->setType("Point");
		pointLight->setId(glm::vec3(0.6f, 0.6f, 0.6f));
		pointLight->setIs(glm::vec3(0.5f, 0.5f, 0.5f));
		pointLight->setPosition(glm::vec3(0.f, 2.f, 15.f));
		pointLight->setLightStatus(true);

		// - Fuente luminosa direccional
		directionalLight->setApplicator(new DirectionalLightApplicator());
		directionalLight->setType("Directional");
		directionalLight->setId(glm::vec3(1.f, 0.81f, 0.62f));
		directionalLight->setIs(glm::vec3(0.6f, 0.6f, 0.6f));
		directionalLight->setDirection(glm::vec3(0.f, -16.f, -15.f));
		directionalLight->setLightStatus(true);

		// - Fuente luminosa spot
		spotLight->setApplicator(new SpotLightApplicator());
		spotLight->setType("Spot");
		spotLight->setId(glm::vec3(1.f, 1.f, 0.5f));
		spotLight->setIs(glm::vec3(0.2f, 0.2f, 0.2f));
		spotLight->setPosition(glm::vec3(0.f, 0.f, 5.f));
		spotLight->setDirection(glm::vec3(0.f, 0.f, -5.f));
		spotLight->setAngle(12.f);
		spotLight->setExponent(8.f);
		spotLight->setLightStatus(false);

		// - Almacenar información inicial de las fuentes luminosas
		ambientLight->setInitialStatus();
		pointLight->setInitialStatus();
		directionalLight->setInitialStatus();
		spotLight->setInitialStatus();

		// - Eliminar fuentes luminosas anteriores
		if (!lights.empty())
		{
			for (unsigned int i = 0; i < lights.size(); i++)
			{
				delete lights[i];
			}

			lights.clear();
		}
		
		// - Añadir fuentes luminosas
		lights.push_back(ambientLight);
		lights.push_back(pointLight);
		lights.push_back(directionalLight);
		lights.push_back(spotLight);
	}
	else if (selectedScene == 1)
	{
		/*
		 * ESCENA 2
		 */

		// - Fuentes luminosas
		LightSource *ambientLight = new LightSource();
		LightSource *pointLight = new LightSource();
		LightSource *directionalLight = new LightSource();
		LightSource *spotLight = new LightSource();

		// - Fuente luminosa ambiente
		ambientLight->setApplicator(new AmbientLightApplicator());
		ambientLight->setType("Ambient");
		ambientLight->setIa(glm::vec3(0.25f, 0.25f, 0.25f));
		ambientLight->setLightStatus(true);

		// - Fuente luminosa puntual
		pointLight->setApplicator(new PointLightApplicator());
		pointLight->setType("Point");
		pointLight->setId(glm::vec3(0.25f, 0.25f, 0.25f));
		pointLight->setIs(glm::vec3(0.25f, 0.25f, 0.25f));
		pointLight->setPosition(glm::vec3(0.f, 2.f, 15.f));
		pointLight->setLightStatus(false);

		// - Fuente luminosa direccional
		directionalLight->setApplicator(new DirectionalLightApplicator());
		directionalLight->setType("Directional");
		directionalLight->setId(glm::vec3(1.f, 0.81f, 0.62f));
		directionalLight->setIs(glm::vec3(0.6f, 0.6f, 0.6f));
		directionalLight->setDirection(glm::vec3(0.f, -16.f, -15.f));
		directionalLight->setLightStatus(false);

		// - Fuente luminosa spot
		spotLight->setApplicator(new SpotLightApplicator());
		spotLight->setType("Spot");
		spotLight->setId(glm::vec3(1.f, 1.f, 1.f));
		spotLight->setIs(glm::vec3(1.f, 1.f, 1.f));
		spotLight->setPosition(glm::vec3(0.f, 20.f, -20.f));
		spotLight->setDirection(glm::vec3(0.f, -20.f, 4.6f));
		spotLight->setAngle(22.f);
		spotLight->setExponent(30.f);
		spotLight->setLightStatus(true);

		// - Almacenar información inicial de las fuentes luminosas
		ambientLight->setInitialStatus();
		pointLight->setInitialStatus();
		directionalLight->setInitialStatus();
		spotLight->setInitialStatus();

		// - Eliminar fuentes luminosas anteriores
		if (!lights.empty())
		{
			for (unsigned int i = 0; i < lights.size(); i++)
			{
				delete lights[i];
			}

			lights.clear();
		}

		// - Añadir fuentes luminosas
		lights.push_back(ambientLight);
		lights.push_back(pointLight);
		lights.push_back(directionalLight);
		lights.push_back(spotLight);
	}
	else if (selectedScene == 2)
	{
		/*
		 * ESCENA 3
		 */

		// - Fuentes luminosas
		LightSource *ambientLight = new LightSource();
		LightSource *pointLight = new LightSource();
		LightSource *directionalLight = new LightSource();
		LightSource *spotLight = new LightSource();

		// - Fuente luminosa ambiente
		ambientLight->setApplicator(new AmbientLightApplicator());
		ambientLight->setType("Ambient");
		ambientLight->setIa(glm::vec3(0.1125f, 0.1125f, 0.1125f));
		ambientLight->setLightStatus(true);

		// - Fuente luminosa puntual
		pointLight->setApplicator(new PointLightApplicator());
		pointLight->setType("Point");
		pointLight->setId(glm::vec3(0.6f, 0.6f, 0.6f));
		pointLight->setIs(glm::vec3(0.5f, 0.5f, 0.5f));
		pointLight->setPosition(glm::vec3(0.f, 2.f, 15.f));
		pointLight->setLightStatus(false);

		// - Fuente luminosa direccional
		directionalLight->setApplicator(new DirectionalLightApplicator());
		directionalLight->setType("Directional");
		directionalLight->setId(glm::vec3(0.875f, 0.62f, 0.35f));
		directionalLight->setIs(glm::vec3(0.6f, 0.6f, 0.6f));
		directionalLight->setDirection(glm::vec3(20.f, -20.f, -20.f));
		directionalLight->setLightStatus(true);

		// - Fuente luminosa spot
		spotLight->setApplicator(new SpotLightApplicator());
		spotLight->setType("Spot");
		spotLight->setId(glm::vec3(1.f, 1.f, 0.5f));
		spotLight->setIs(glm::vec3(0.2f, 0.2f, 0.2f));
		spotLight->setPosition(glm::vec3(0.f, 0.f, 0.f));
		spotLight->setDirection(glm::vec3(0.f, 0.f, -5.f));
		spotLight->setAngle(12.f);
		spotLight->setExponent(8.f);
		spotLight->setLightStatus(false);

		// - Almacenar información inicial de las fuentes luminosas
		ambientLight->setInitialStatus();
		pointLight->setInitialStatus();
		directionalLight->setInitialStatus();
		spotLight->setInitialStatus();

		// - Eliminar fuentes luminosas anteriores
		if (!lights.empty())
		{
			for (unsigned int i = 0; i < lights.size(); i++)
			{
				delete lights[i];
			}

			lights.clear();
		}

		// - Añadir fuentes luminosas
		lights.push_back(ambientLight);
		lights.push_back(pointLight);
		lights.push_back(directionalLight);
		lights.push_back(spotLight);
	}
}

// - Reinicio de fuentes luminosas a su estado por defecto
void Renderer::lightSourceReset(std::string type)
{
	if (type == "Ambient")
	{
		lights[0]->resetInitialStatus();
	}
	else if (type == "Point")
	{
		lights[1]->resetInitialStatus();
	}
	else if (type == "Directional")
	{
		lights[2]->resetInitialStatus();
	}
	else if (type == "Spot")
	{
		lights[3]->resetInitialStatus();
	}
}

// - Activar/desactivar modos de dibujo
void Renderer::setRenderingMode(std::string mode)
{
	// - Desactivar Post-procesamiento
	enabledPostProcessing = false;

	if (mode == "realistic")
	{
		// - Activar modo de dibujado: Realista
		enabledRealistic = true;
		selectedTechnique = 0;
	}

	if (mode == "monochrome")
	{
		// - Activar modo de dibujado: Monochrome
		enabledMonochrome = true;
		selectedTechnique = 1;
	}

	if (mode == "celShading")
	{
		// - Activar modo de dibujado: Cel-Shading
		enabledCelShading = true;
		selectedTechnique = 2;
	}

	if (mode == "halftone")
	{
		enabledPostProcessing = true;

		// - Activar modo de dibujado: Halftone
		enabledHalftone = true;
		selectedTechnique = 3;
	}

	if (mode == "dithering")
	{
		enabledPostProcessing = true;

		// - Activar modo de dibujado: Dithering
		enabledDithering = true;
		selectedTechnique = 4;
	}

	if (mode == "hatching")
	{
		// - Activar modo de dibujado: Hatching
		enabledHatching = true;
		selectedTechnique = 5;
	}

	if (mode == "charcoal")
	{
		enabledPostProcessing = true;

		// - Activar modo de dibujado: Charcoal
		enabledCharcoal = true;
		selectedTechnique = 6;
	}

	if (mode == "goochShading")
	{
		// - Activar modo de dibujado: Gooch Shading
		enabledGoochShading = true;
		selectedTechnique = 7;
	}

	if (mode == "painterly")
	{
		enabledPostProcessing = true;

		// - Activar modo de dibujado: Painterly (óleo)
		enabledPainterly = true;
		selectedTechnique = 8;
	}

	if (mode == "pixelArt")
	{
		enabledPostProcessing = true;

		// - Activar modo de dibujado: Pixel Art
		enabledPixelArt = true;
		selectedTechnique = 9;
	}
}

// - Desactivar todos los modos de dibujado
void Renderer::resetAllRenderingModes()
{
	enabledRealistic = false;
	enabledMonochrome = false;
	enabledCelShading = false;
	enabledHatching = false;
	enabledCharcoal = false;
	enabledGoochShading = false;

	enabledHalftone = false;
	enabledDithering = false;
	enabledPixelArt = false;
	enabledPainterly = false;

	enabledPostProcessing = false;
}

// - Método para dibujar la escena
void Renderer::render()
{
	// - Reiniciar contador de luces activadas
	numberOfLightsEnabled = 0;

	// - Comprobar estado de activación de las fuentes luminosas
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		if (lights[i]->isLightEnabled())
		{
			numberOfLightsEnabled++;
		}
	}
	
	// - Comprobar estado del blending
	if (numberOfLightsEnabled == 0)
	{
		glDisable(GL_BLEND);
	}
	else
	{
		glEnable(GL_BLEND);
	}

	// - Dibujar escena de forma realista
	if (enabledRealistic)
	{
		realistic();
	}

	// - Dibujar escena de forma monocromática
	if (enabledMonochrome)
	{
		monochrome();
	}

	// - Dibujar escena usando la técnica Cel-Shading
	if (enabledCelShading)
	{
		celShading();
	}

	// - Dibujar escena usando la técnica Halftone
	if (enabledHalftone)
	{
		halftone();
	}

	// - Dibujar escena usando la técnica Cross-Hatching
	if (enabledDithering)
	{
		dithering();
	}

	// - Dibujar escena usando la técnica Hatching
	if (enabledHatching)
	{
		hatching();
	}

	// - Dibujar escena usando la técnica Charcoal
	if (enabledCharcoal)
	{
		charcoal();
	}

	// - Dibujar escena usando la técnica Gooch Shading
	if (enabledGoochShading)
	{
		goochShading();
	}

	// - Dibujar escena usando la técnica Painterly (óleo)
	if (enabledPainterly)
	{
		painterly();
	}

	// - Dibujar escena usando la técnica Pixel Art
	if (enabledPixelArt)
	{
		pixelArt();
	}
}

/*
 **********************************************
		      RENDERING (MODELOS)
 **********************************************
 */

// - Rendering realista
void Renderer::realistic()
{
	// - Flag para comprobar la primera fuenta activa
	bool firstLightEnabled = false;

	// - Iluminación
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		// - Dibujar contorno básico
		basicOutline();

		// - Dibujar contorno avanzado
		advancedOutline();

		// - Activar shader
		realisticShader.use();

		// - Aplicar fuente luminosa si está activada
		if (lights[i]->isLightEnabled())
		{
			if (!firstLightEnabled)
			{
				// - Si es la primera, activar este modo de mezcla
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstLightEnabled = true;
			}
			else
			{
				// - Para el resto de fuentes luminosas activar este otro modo
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			lights[i]->apply(realisticShader);
		}

		// - Dibujar la escena
		currentScene->drawRealistic(realisticShader, currentScene->getModelMatrix(), 
								    camera->getViewMatrix(), camera->getProjectionMatrix());
	}

	// - Dibujado de skybox
	realisticSkyboxShader.use();
	glm::mat4 skyboxVP = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	skybox->drawRealistic(realisticSkyboxShader, skyboxVP);
}

// - Rendering Monochrome
void Renderer::monochrome()
{
	// - Flag para comprobar la primera fuenta activa
	bool firstLightEnabled = false;

	// - Iluminación
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		// - Dibujar contorno básico
		basicOutline();

		// - Dibujar contorno avanzado
		advancedOutline();

		// - Activar shader
		monochromeShader.use();

		// - Aplicar fuente luminosa si está activada
		if (lights[i]->isLightEnabled())
		{
			if (!firstLightEnabled)
			{
				// - Si es la primera, activar este modo de mezcla
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstLightEnabled = true;
			}
			else
			{
				// - Para el resto de fuentes luminosas activar este otro modo
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			lights[i]->apply(monochromeShader);
		}

		// - Dibujar la escena
		currentScene->drawMonochrome(monochromeShader, currentScene->getModelMatrix(), camera->getViewMatrix(),
									camera->getProjectionMatrix());
	}

	// - Dibujado de skybox
	realisticSkyboxShader.use();
	glm::mat4 skyboxVP = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	skybox->drawRealistic(realisticSkyboxShader, skyboxVP);
}

// - Técnica de rendering NPR: Cel-Shading
void Renderer::celShading()
{
	// - Flag para comprobar la primera fuenta activa
	bool firstLightEnabled = false;

	// - Intensidad de la fuente luminosa ambiente
	glm::vec3 Ia = glm::vec3(0.f);

	// - Iluminación
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		// - Dibujar contorno básico
		basicOutline();

		// - Dibujar contorno avanzado
		advancedOutline();

		// - Activar shader
		celShadingShader.use();

		// - Aplicar fuente luminosa si está activada
		if (lights[i]->isLightEnabled())
		{
			if (!firstLightEnabled)
			{
				// - Si es la primera, activar este modo de mezcla
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstLightEnabled = true;
			}
			else
			{
				// - Para el resto de fuentes luminosas activar este otro modo
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			// - Obtener intensidad de la fuente ambiente para calcular color final del skybox
			if (lights[i]->getType() == "Ambient")
			{
				Ia = lights[i]->getIa() / glm::vec3(2.f);
			}

			lights[i]->apply(celShadingShader);
		}

		// - Dibujar la escena
		currentScene->drawCelShading(celShadingShader, currentScene->getModelMatrix(), camera->getViewMatrix(),
									camera->getProjectionMatrix());
	}

	// - Dibujado de skybox
	celShadingSkyboxShader.use();
	glm::mat4 skyboxVP = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	skybox->drawCelShading(celShadingSkyboxShader, skyboxVP, Ia);
}

// - Técnica de rendering NPR: Hatching
void Renderer::hatching()
{
	// - Flag para comprobar la primera fuenta activa
	bool firstLightEnabled = false;

	// - Iluminación
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		// - Dibujar contorno básico
		basicOutline();

		// - Dibujar contorno avanzado
		advancedOutline();

		// - Activar shader
		hatchingShader.use();

		// - Aplicar fuente luminosa si está activada
		if (lights[i]->isLightEnabled())
		{
			if (!firstLightEnabled)
			{
				// - Si es la primera, activar este modo de mezcla
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstLightEnabled = true;
			}
			else
			{
				// - Para el resto de fuentes luminosas activar este otro modo
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			lights[i]->apply(hatchingShader);
		}

		// - Dibujar la escena
		currentScene->drawHatching(hatchingShader, currentScene->getModelMatrix(), camera->getViewMatrix(),
								  camera->getProjectionMatrix());
	}

	// - Dibujado de skybox
	hatchingSkyboxShader.use();
	glm::mat4 skyboxVP = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	skybox->drawHatching(hatchingSkyboxShader, skyboxVP);
}

// - Técnica de rendering NPR: Gooch Shading
void Renderer::goochShading()
{
	// - Flag para comprobar la primera fuenta activa
	bool firstLightEnabled = false;

	// - Iluminación
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		// - Dibujar contorno básico
		basicOutline();

		// - Dibujar contorno avanzado
		advancedOutline();

		// - Activar shader
		goochShadingShader.use();

		// - Aplicar fuente luminosa si está activada
		if (lights[i]->isLightEnabled())
		{
			if (!firstLightEnabled)
			{
				// - Si es la primera, activar este modo de mezcla
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				firstLightEnabled = true;
			}
			else
			{
				// - Para el resto de fuentes luminosas activar este otro modo
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			lights[i]->apply(goochShadingShader);
		}

		// - Dibujar la escena
		currentScene->drawGoochShading(goochShadingShader, currentScene->getModelMatrix(), camera->getViewMatrix(),
									  camera->getProjectionMatrix());
	}

	// - Dibujado de skybox
	realisticSkyboxShader.use();
	glm::mat4 skyboxVP = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	skybox->drawRealistic(realisticSkyboxShader, skyboxVP);
}

/*
 **********************************************
		 RENDERING (POST-PROCESAMIENTO)
 **********************************************
 */

// - Técnica de rendering NPR: Halftone
void Renderer::halftone()
{
	// - 1º PASADA: Dibujar la escena en la textura del FBO

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Dibujar la escena usando la técnica Cel-Shading (o Toon Shading)
	celShading();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - 2º PASADA: Usar la textura en un quad que se mostrará en el FrameBuffer principal

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Enlazar texturas de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();

	fbo->bindDepthBuffer();

	// - Dibujar Quad con la textura de la escena
	halftoneShader.use();
	quad->drawHalftone(halftoneShader, 0);

	// - Desenlazar texturas de color y profundiad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();
}

// - Técnica de rendering NPR: Dithering
void Renderer::dithering()
{
	// - 1º PASADA: Dibujar la escena en la textura del FBO

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Dibujar la escena de forma realista
	realistic();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - 2º PASADA: Usar la textura en un quad que se mostrará en el FrameBuffer principal

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Enlazar texturas de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();

	fbo->bindDepthBuffer();

	// - Dibujar Quad con la textura de la escena
	ditheringShader.use();
	quad->drawDithering(ditheringShader, 0);

	// - Desenlazar texturas de color y profundiad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();
}

// - Técnica de rendering NPR: PixelArt
void Renderer::pixelArt()
{
	// - 1º PASADA: Dibujar la escena en la textura del FBO

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Dibujar la escena de forma realista
	realistic();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - 2º PASADA: Usar la textura en un quad que se mostrará en el FrameBuffer principal

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Enlazar texturas de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();

	fbo->bindDepthBuffer();

	// - Dibujar Quad con la textura de la escena
	pixelArtShader.use();
	quad->drawPixelArt(pixelArtShader, 0);

	// - Desenlazar texturas de color y profundiad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();
}

// - Técnica de rendering NPR: Painterly (óleo)
void Renderer::painterly()
{
	// - 1º PASADA: Dibujar la escena en la textura del FBO

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Dibujar la escena de forma realista
	realistic();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - 2º PASADA: Usar la textura en un quad que se mostrará en el FrameBuffer principal

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Enlazar texturas de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();

	fbo->bindDepthBuffer();

	// - Dibujar Quad con la textura de la escena
	painterlyShader.use();
	quad->drawPainterly(painterlyShader, 0);

	// - Desenlazar texturas de color y profundiad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();
}

// - Técnica de rendering NPR: Charcoal (carboncillo)
void Renderer::charcoal()
{
	// - 1º PASADA: Dibujar la escena en la textura del FBO

	// - Enlazar FBO
	fbo->bindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Dibujar la escena de forma realista
	realistic();

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fbo->unbindFrameBuffer();

	// - 2º PASADA: Usar la textura en un quad que se mostrará en el FrameBuffer principal

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Enlazar texturas de color y profundidad
	fbo->activeTextureUnit(0);
	fbo->bindRenderTexture();

	fbo->bindDepthBuffer();

	// - Dibujar Quad con la textura de la escena
	charcoalShader.use();
	quad->drawCharcoal(charcoalShader, 0);

	// - Desenlazar texturas de color y profundiad
	fbo->unbindRenderTexture();
	fbo->unbindDepthBuffer();
}

/*
 **********************************************
				   CONTORNOS
 **********************************************
 */

// - Dibujar contorno básico alrededor de los objetos
void Renderer::basicOutline()
{
	// - Activar Front Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// - Dibujar contornos para los elementos de la escena que lo tengan activado
	for (int i = 0; i < currentScene->getNumElements(); i++)
	{
		if (currentScene->getElement(i)->getBasicOutline().enabled)
		{
			// - Dibujado de contornos (modelos escalados con un color específico)
			basicOutlineShader.use();
			currentScene->getElement(i)->drawBasicOutline(basicOutlineShader, 
														  currentScene->getElement(i)->getModelMatrix(),
														  camera->getViewMatrix(), 
														  camera->getProjectionMatrix());
		}
	}
	
	// - Desactivar Face-culling
	glDisable(GL_CULL_FACE);
}

// - Dibujar contorno avanzado alrededor de los objetos
void Renderer::advancedOutline()
{
	// - Dibujar contornos para los elementos de la escena que lo tengan activado
	for (int i = 0; i < currentScene->getNumElements(); i++)
	{
		if (currentScene->getElement(i)->getAdvancedOutline().enabled)
		{
			// - Dibujado de contornos (usando adyacencias de triángulos en las mallas de los modelos)
			advancedOutlineShader.use();
			currentScene->getElement(i)->drawAdvancedOutline(advancedOutlineShader,
															 currentScene->getElement(i)->getModelMatrix(),
															 camera->getViewMatrix(),
															 camera->getProjectionMatrix());
		}
	}
}

// - Activar/desactivar dibujado de contornos
void Renderer::toggleBasicOutline()
{
	currentElement->getBasicOutline().enabled = !currentElement->getBasicOutline().enabled;
}

// - Activar/desactivar dibujado de contornos
void Renderer::toggleAdvancedOutline()
{
	currentElement->getAdvancedOutline().enabled = !currentElement->getAdvancedOutline().enabled;
}

/*
 **********************************************
				    CÁMARA
 **********************************************
 */

// - Cambiar posición de la cámara
void Renderer::setCameraPosition(glm::vec3 position)
{
	camera->setPosition(position);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Asignar el aspect ratio a la cámara (al redimensionar ventana)
void Renderer::setCameraAspect(int width, int height)
{
	camera->setAspect(width, height);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'pan' de la cámara
void Renderer::cameraPanMovement(float rotX)
{
	camera->pan(rotX);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'tilt' de la cámara
void Renderer::cameraTiltMovement(float rotY)
{
	camera->tilt(rotY);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'truck' de la cámara
void Renderer::cameraTruckMovement(float movX)
{
	camera->truck(movX);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'boom' de la cámara
void Renderer::cameraBoomMovement(float movY)
{
	camera->boom(movY);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'dolly' de la cámara
void Renderer::cameraDollyMovement(float movZ)
{
	camera->dolly(movZ);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Movimiento 'orbit' de la cámara
void Renderer::cameraOrbitMovement(float angle)
{
	camera->orbit(angle);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Zoom de la cámara
void Renderer::cameraZoom(float yoffset)
{
	camera->zoom(yoffset);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

// - Resetear cámara
void Renderer::cameraReset()
{
	camera->reset();
	camera->setAspect(viewportWidth, viewportHeight);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}

/*
 **********************************************
					 GUI
 **********************************************
 */

 // - Mostrar/Ocultar ventana principal
void Renderer::toggleWindow()
{
	enabledMainWindowGUI = !enabledMainWindowGUI;

	if (!enabledMainWindowGUI)
	{
		enabledScroll = false;
	}
}

// - Saber si está permitido o no hacer scroll con el ratón
bool Renderer::isScrollEnabled()
{
	return enabledScroll;
}

 // - Crear y renderizar la GUI principal de la aplicación
void Renderer::renderMainGUI()
{
	if (enabledMainWindowGUI)
	{
		// - Iniciar definición de la ventana principal (no se permite moverla ni cambiar su tamaño)
		if (!ImGui::Begin("MAIN MENU", &enabledMainWindowGUI, ImGuiWindowFlags_NoMove | 
															  ImGuiWindowFlags_NoResize))
		{
			ImGui::End();
		}
		else
		{
			// - Fijar tamaño
			ImGui::SetWindowSize(ImVec2(viewportWidth / 2.75f, viewportHeight));

			// - Fijar posición
			ImGui::SetWindowPos(ImVec2(0, 0));

			// - Activar/desactivar scroll sobre la GUI => Desactivar/activar zoom haciendo scroll (ratón)
			//   cuando el cursor esté encima de la ventana o algún elemento
			if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
			{
				enabledScroll = true;
			}
			else
			{
				enabledScroll = false;
			}

			// - Escenas
			scenesGUI();
			
			// - Cámara
			cameraGUI();
			
			// - Iluminación
			lightingGUI();

			// - Rendering
			renderingGUI();

			// - Miscelánea
			miscellaneousGUI();

			// - Terminar definición de la ventana
			ImGui::End();
		}
	}

	// - Renderizar GUI
	ImGui::Render();
}

// - GUI de escenas
void Renderer::scenesGUI()
{
	// - TODO: SELECTOR DE ESCENAS
	if (ImGui::CollapsingHeader("Scenes"))
	{
		// - Listado de técnicas de rendering
		static const char *scenes[]
		{
			"Scene 1",
			"Scene 2",
			"Scene 3"
		};

		// - Texto (escena seleccionada)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected scene:");

		// - Crear ListBox con las escenas
		ImGui::ListBox("##ListBoxScenes", &selectedScene, scenes,
					   IM_ARRAYSIZE(scenes), (sizeof(scenes) / sizeof(*scenes) + 1));

		// - Elementos de la escena 1
		static const char* scene1Names[]
		{
			"Iron Man",
			"Captain America",
			"Hulk",
			"Spider-Man",
			"Floor"
		};

		// - Elementos de la escena 2
		static const char* scene2Names[]
		{
			"Room",
			"Fruit bowl"
		};

		// - Elementos de la escena 3
		static const char* scene3Names[]
		{
			"Island",
			"Treasure chest",
			"Statue"
		};

		bool sceneSwitched = false;

		// - Elementos por escena
		switch (selectedScene)
		{
			case 0:

				if (currentScene == scene1)
				{
					// - Separador
					ImGui::Separator();

					// - Texto (elemento seleccionado)
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected element:");

					// - Crear ListBox de los elementos de la escena 1
					ImGui::ListBox("##ListBoxElements1", &selectedElementScene1, scene1Names,
								   IM_ARRAYSIZE(scene1Names), (sizeof(scene1Names) / sizeof(*scene1Names) + 1));

					// - Actualizar elemento seleccionado actualmente
					currentElement = currentScene->getElement(selectedElementScene1);
				}
				else
				{
					// - Añadir espacio
					ImGui::Spacing();

					// - Botón para cambiar la escena
					if (ImGui::Button("Change scene"))
					{
						setupScene(0);
					}
				}

				break;

			case 1:

				if (currentScene == scene2)
				{
					// - Separador
					ImGui::Separator();

					// - Texto (elemento seleccionado)
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected element:");

					// - Crear ListBox de los elementos de la escena 2
					ImGui::ListBox("##ListBoxElements2", &selectedElementScene2, scene2Names,
								   IM_ARRAYSIZE(scene2Names), (sizeof(scene2Names) / sizeof(*scene2Names) + 1));

					// - Actualizar elemento seleccionado actualmente
					currentElement = currentScene->getElement(selectedElementScene2);
				}
				else
				{
					// - Añadir espacio
					ImGui::Spacing();

					// - Botón para cambiar la escena
					if (ImGui::Button("Change scene"))
					{
						setupScene(1);
					}
				}

				break;

			case 2:

				if (currentScene == scene3)
				{
					// - Separador
					ImGui::Separator();

					// - Texto (elemento seleccionado)
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected element:");

					// - Crear ListBox de los elementos de la escena 2
					ImGui::ListBox("##ListBoxElements3", &selectedElementScene3, scene3Names,
								   IM_ARRAYSIZE(scene3Names), (sizeof(scene3Names) / sizeof(*scene3Names) + 1));

					// - Actualizar elemento seleccionado actualmente
					currentElement = currentScene->getElement(selectedElementScene3);
				}
				else
				{
					// - Añadir espacio
					ImGui::Spacing();

					// - Botón para cambiar la escena
					if (ImGui::Button("Change scene"))
					{
						setupScene(2);
					}
				}

				break;
		}
	}
}

// - GUI de cámara
void Renderer::cameraGUI()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		// - Comprobar límites de movimiento (posición y lookAt) en ejes
		camera->checkLimits();

		// - Asignar anchura a los siguientes elementos de la GUI creados
		ImGui::PushItemWidth(ImGui::GetWindowSize().x / 4.f);

		// - Posición (X)
		ImGui::SliderFloat("###PositionX##Camera", &camera->getPosition().x,
						   camera->getMinLimitPosition().x, camera->getMaxLimitPosition().x, "X: %.1f");

		// - Misma línea
		ImGui::SameLine();

		// - Posición (Y)
		ImGui::SliderFloat("###PositionY##Camera", &camera->getPosition().y,
						   camera->getMinLimitPosition().y, camera->getMaxLimitPosition().y, "Y: %.1f");

		// - Misma línea
		ImGui::SameLine();

		// - Posición (Z)
		ImGui::SliderFloat("Position##Camera", &camera->getPosition().z,
						   camera->getMinLimitPosition().z, camera->getMaxLimitPosition().z, "Z: %.1f");

		// - LookAt (X)
		ImGui::SliderFloat("###LookAtX##Camera", &camera->getLookAt().x,
						   camera->getMinLimitLookAt().x, camera->getMaxLimitLookAt().x, "X: %.1f");

		// - Misma línea
		ImGui::SameLine();

		// - LookAt (Y)
		ImGui::SliderFloat("###LookAtY##Camera", &camera->getLookAt().y,
						   camera->getMinLimitLookAt().y, camera->getMaxLimitLookAt().y, "Y: %.1f");

		// - Misma línea
		ImGui::SameLine();

		// - LookAt (Z)
		ImGui::SliderFloat("LookAt##Camera", &camera->getLookAt().z,
						   camera->getMinLimitLookAt().z, camera->getMaxLimitLookAt().z, "Z: %.1f");

		// - Revertir anchura
		ImGui::PopItemWidth();

		// - Campo de visión horizontal
		ImGui::SliderFloat("Field of view (FOV)##Camera", &camera->getFovX(),
						   camera->getMinLimitFOVX(), camera->getMaxLimitFOVX(), "%.0f degrees");

		// - ZNear
		ImGui::SliderFloat("Near plane (zNear)##Camera", &camera->getZNear(),
						   0.0001f, 1.f, "%.4f");

		// - ZFar
		ImGui::SliderFloat("Far plane (zFar)##Camera", &camera->getZFar(),
						   10.f, 100000.f, "%.0f");

		// - Actualizar matriz de visión y proyección
		camera->calculateViewMatrix();
		camera->calculateProjectionMatrix();

		mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

		// - Añadir espacio
		ImGui::Spacing();

		// - Botón para restaurar a estado por defecto
		if (ImGui::Button("Default##Camera"))
		{
			cameraReset();
		}
	}
}

// - GUI de iluminación
void Renderer::lightingGUI()
{
	if (ImGui::CollapsingHeader("Lighting"))
	{
		// - Intensidad ambiente
		ImGui::ColorEdit3("Ambient intensity##Ambient", &lights[0]->getIa()[0]);

		// - Añadir espacio
		ImGui::Spacing();

		// - Botón para restaurar a estado por defecto
		if (ImGui::Button("Default##Ambient"))
		{
			lightSourceReset("Ambient");
		}

		// - Separador
		ImGui::Separator();

		// - Fuente luminosa puntual
		ImGui::Checkbox("Point light", &lights[1]->isLightEnabled());

		if (lights[1]->isLightEnabled())
		{
			// - Intensidad difusa
			ImGui::ColorEdit3("Diffuse intensity##Point", &lights[1]->getId()[0]);

			// - Intensidad especular
			ImGui::ColorEdit3("Specular intensity##Point", &lights[1]->getIs()[0]);

			// - Posición
			ImGui::SliderFloat3("Position##Point", &lights[1]->getPosition()[0], 
								-20.f, 20.f, "%.1f");

			// - Añadir espacio
			ImGui::Spacing();

			// - Botón para restaurar a estado por defecto
			if (ImGui::Button("Default##Point"))
			{
				lightSourceReset("Point");
			}

			// - Separador
			ImGui::Separator();
		}

		// - Fuente luminosa direccional
		ImGui::Checkbox("Directional light", &lights[2]->isLightEnabled());

		if (lights[2]->isLightEnabled())
		{
			// - Intensidad difusa
			ImGui::ColorEdit3("Diffuse intensity##Directional", &lights[2]->getId()[0]);

			// - Intensidad especular
			ImGui::ColorEdit3("Specular intensity##Directional", &lights[2]->getIs()[0]);

			// - Dirección
			ImGui::SliderFloat3("Direction##Directional", &lights[2]->getDirection()[0],
								-20.f, 20.f, "%.1f");

			// - Añadir espacio
			ImGui::Spacing();

			// - Botón para restaurar a estado por defecto
			if (ImGui::Button("Default##Directional"))
			{
				lightSourceReset("Directional");
			}

			// - Separador
			ImGui::Separator();
		}

		// - Fuente luminosa ambiente
		ImGui::Checkbox("Spot light", &lights[3]->isLightEnabled());

		if (lights[3]->isLightEnabled())
		{
			// - Intensidad difusa
			ImGui::ColorEdit3("Diffuse intensity##Spot", &lights[3]->getId()[0]);

			// - Intensidad especular
			ImGui::ColorEdit3("Specular intensity##Spot", &lights[3]->getIs()[0]);

			// - Posición
			ImGui::SliderFloat3("Position##Spot", &lights[3]->getPosition()[0],
								-20.f, 20.f, "%.1f");

			// - Dirección
			ImGui::SliderFloat3("Direction##Spot", &lights[3]->getDirection()[0],
								-20.f, 20.f, "%.1f");

			// - Dirección
			ImGui::SliderFloat("Angle##Spot", &lights[3]->getAngle(),
							   0.f, 90.f, "%.1f");

			ImGui::SliderFloat("Exponent##Spot", &lights[3]->getExponent(),
							   1.f, 256.f, "%.1f");

			// - Añadir espacio
			ImGui::Spacing();

			// - Botón para restaurar a estado por defecto
			if (ImGui::Button("Default##Spot"))
			{
				lightSourceReset("Spot");
			}
		}
	}
}

// - GUI de rendering
void Renderer::renderingGUI()
{
	if (ImGui::CollapsingHeader("Rendering"))
	{
		// - Texto (técnicas de rendering)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rendering techniques:");

		// - Listado de técnicas de rendering
		static const char *techniques[]
		{
			"Realistic",
			"Monochrome",
			"Cel-Shading",
			"Halftone (Cel-Shading)",
			"Dithering",
			"Hatching",
			"Charcoal",
			"Gooch Shading",
			"Painterly",
			"Pixel Art"
		};

		// - Crear ListBox con las técnicas
		ImGui::ListBox("##ListBoxRendering", &selectedTechnique, techniques,
					   IM_ARRAYSIZE(techniques), (sizeof(techniques) / sizeof(*techniques) + 1));

		// - Configuración del dibujado de contornos (si el modelo no es un plano)
		if (!currentElement->elementIsPlane())
		{
			// - Separador
			ImGui::Separator();

			// - Texto (Outline)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Outline (Selected element):");

			// - Contorno básico
			ImGui::Checkbox("Basic outline", &currentElement->getBasicOutline().enabled);

			if (currentElement->getBasicOutline().enabled)
			{
				ImGui::ColorEdit3("Color##Basic", &currentElement->getBasicOutline().color[0]);

				ImGui::SliderFloat("Thickness##Basic",
								   &currentElement->getBasicOutline().thickness,
								   0.f, 0.05f, "%.4f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Basic"))
				{
					currentElement->resetBasicOutline();
				}
			}

			// - Separador
			ImGui::Separator();

			// - Contorno avanzado
			ImGui::Checkbox("Advanced outline", &currentElement->getAdvancedOutline().enabled);

			if (currentElement->getAdvancedOutline().enabled)
			{
				ImGui::ColorEdit3("Color##Advanced", &currentElement->getAdvancedOutline().color[0]);

				ImGui::SliderFloat("Thickness##Advanced",
								   &currentElement->getAdvancedOutline().thickness,
								   0.f, 0.05f, "%.4f");

				ImGui::SliderFloat("Extension", &currentElement->getAdvancedOutline().extension,
								   0.f, 0.05f, "%.4f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Advanced"))
				{
					currentElement->resetAdvancedOutline();
				}
			}
		}

		// - Constante cero
		static float zero = 0.f;

		// - Selector de técnica
		switch (selectedTechnique)
		{
			case 0:
				resetAllRenderingModes();
				setRenderingMode("realistic");

				break;

			case 1:
				resetAllRenderingModes();
				setRenderingMode("monochrome");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica)
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Selected element):");

				// - Color Editor (Ka)
				ImGui::ColorEdit3("Ambient color (Ka)",
								  &currentElement->getMonochromeTechnique().material.getKa()[0]);

				// - Color Editor (Kd)
				ImGui::ColorEdit3("Diffuse color (Kd)",
								  &currentElement->getMonochromeTechnique().material.getKd()[0]);

				// - Color Editor (Ks)
				ImGui::ColorEdit3("Specular color (Ks)",
								  &currentElement->getMonochromeTechnique().material.getKs()[0]);

				// - Slider (Shininess)
				ImGui::SliderFloat("Shininess", 
								   &currentElement->getMonochromeTechnique().material.getShininess(),
								   1.0f, 128.0f, "%.1f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Monochrome"))
				{
					currentElement->resetMonochromeTechnique();
				}

				break;

			case 2:
				resetAllRenderingModes();
				setRenderingMode("celShading");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Elemento
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Selected element):");

				// - SliderFloat (número de tonos)
				ImGui::SliderFloat("Number of tones##Element", 
								   &currentElement->getCelShadingTechnique().tones,
								   2.f, 16.f, "%.0f");

				// - SliderFloat (siluetado)
				ImGui::SliderFloat("Silhouetting factor",
								   &currentElement->getCelShadingTechnique().silhouettingFactor,
								   0.f, 0.7f, "%.2f");

				// - Separador
				ImGui::Spacing();

				// - Texto (parámetros de la técnica): Skybox
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Skybox):");

				// - SliderFloat (número de tonos del skybox)
				ImGui::SliderFloat("Number of tones##Skybox",
								   &skybox->getCelShadingTechnique().tones,
								   2.f, 32.f, "%.0f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##CelShading"))
				{
					currentElement->resetCelShadingTechnique();
					skybox->resetCelShadingTechnique();
				}

				break;

			case 3:
				resetAllRenderingModes();
				setRenderingMode("halftone");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Escena
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Scene):");

				// - SliderFloat (umbral)
				ImGui::SliderFloat("Threshold##Halftone",
								   &quad->getHalftoneTechnique().threshold,
								   0.f, 0.8f, "%.1f");

				// - SliderFloat (factor de intensidad)
				ImGui::SliderFloat("Intensity##Halftone",
								   &quad->getHalftoneTechnique().intensity,
								   0.2f, 2.f, "%.3f");

				// - SliderFloat (tamaño)
				ImGui::SliderFloat("Size##Halftone",
								   &quad->getHalftoneTechnique().size,
								   1.f, 4.f, "%.1f");

				// - SliderFloat (color)
				ImGui::ColorEdit3("Color##Halftone", &quad->getHalftoneTechnique().color[0]);

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Halftone"))
				{
					quad->resetHalftoneTechnique();
				}

				break;

			case 4:
				resetAllRenderingModes();
				setRenderingMode("dithering");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Escena
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Scene):");

				// - CheckBox (blanco y negro)
				ImGui::Checkbox("Use scene's color ##Dithering",
								&quad->getDitheringTechnique().useSceneColor);

				// - Añadir espacio
				ImGui::Spacing();

				// - Asignar anchura a los siguientes elementos de la GUI creados
				ImGui::PushItemWidth(ImGui::GetWindowSize().x / 2.5f);

				// - SliderFloat (umbral 1)
				ImGui::SliderFloat("###Threshold1##Dithering",
								   &quad->getDitheringTechnique().threshold1,
								   0.75f, 1.f, "Threshold 1: %.2f");

				// - Misma línea
				ImGui::SameLine();

				// - SliderFloat (intensidad umbral 1)
				ImGui::SliderFloat("###IntensityThreshold1##Dithering",
								   &quad->getDitheringTechnique().intensityThreshold1,
								   0.f, 1.f, "Intensity 1: %.2f");

				// - SliderFloat (umbral 2)
				ImGui::SliderFloat("###Threshold2##Dithering",
								   &quad->getDitheringTechnique().threshold2,
								   0.5f, 0.75f, "Threshold 2: %.2f");

				// - Misma línea
				ImGui::SameLine();

				// - SliderFloat (intensidad umbral 2)
				ImGui::SliderFloat("###IntensityThreshold2##Dithering",
								   &quad->getDitheringTechnique().intensityThreshold2,
								   0.f, 1.f, "Intensity 2: %.2f");

				// - SliderFloat (umbral 3)
				ImGui::SliderFloat("###Threshold3##Dithering",
								   &quad->getDitheringTechnique().threshold3,
								   0.25f, 0.5f, "Threshold 3: %.2f");

				// - Misma línea
				ImGui::SameLine();

				// - SliderFloat (intensidad umbral 3)
				ImGui::SliderFloat("###IntensityThreshold3##Dithering",
								   &quad->getDitheringTechnique().intensityThreshold3,
								   0.f, 1.f, "Intensity 3: %.2f");

				// - SliderFloat (umbral 4)
				ImGui::SliderFloat("###Threshold4##Dithering",
								   &quad->getDitheringTechnique().threshold4,
								   0.f, 0.25f, "Threshold 4: %.2f");

				// - Misma línea
				ImGui::SameLine();

				// - SliderFloat (intensidad umbral 4)
				ImGui::SliderFloat("###IntensityThreshold4##Dithering",
								   &quad->getDitheringTechnique().intensityThreshold4,
								   0.f, 1.f, "Intensity 4: %.2f");

				// - Añadir espacio
				ImGui::Spacing();

				// - SliderFloat (anchura)
				ImGui::SliderFloat("###Width##Dithering",
								   &quad->getDitheringTechnique().width,
								   1.f, 4.f, "Width: %.1f");

				// - Misma línea
				ImGui::SameLine();

				// - SliderFloat (densidad)
				ImGui::SliderFloat("###Density##Dithering",
								   &quad->getDitheringTechnique().density,
								   5.f, 8.f, "Density: %.1f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Dithering"))
				{
					quad->resetDitheringTechnique();
				}

				break;

			case 5:
				resetAllRenderingModes();
				setRenderingMode("hatching");

				// - Añadir nueva línea de espacio
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Elemento
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Selected element):");

				// - SliderFloat (densidad de hatching)
				ImGui::SliderFloat("Density##HatchingElement",
								   &currentElement->getHatchingTechnique().density,
								   2.f, 16.f, "%.0f");

				// - SliderFloat (ángulo de rotación en eje X)
				ImGui::SliderFloat("Rotation (X Axis)##Element",
								   &currentElement->getHatchingTechnique().rotationAngleX,
								   0.f, 360.f, "%0.f degrees");

				// - SliderFloat (ángulo de rotación en eje Y)
				ImGui::SliderFloat("Rotation (Y Axis)##Element",
								   &currentElement->getHatchingTechnique().rotationAngleY,
								   0.f, 360.f, "%0.f degrees");

				// - Separador
				ImGui::Spacing();

				// - Texto (parámetros de la técnica): Skybox
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Skybox):");

				// - SliderFloat (densidad de hatching en el Skybox)
				ImGui::SliderFloat("Density##HatchingSkybox",
								   &skybox->getHatchingTechnique().density,
								   2.f, 32.f, "%0.f");

				// - SliderFloat (ángulo de rotación en eje X)
				ImGui::SliderFloat("Rotation (X Axis)##Skybox",
								   &skybox->getHatchingTechnique().rotationAngleX,
								   0.f, 360.f, "%0.f degrees");

				// - SliderFloat (ángulo de rotación en eje Y)
				ImGui::SliderFloat("Rotation (Y Axis)##Skybox",
								   &skybox->getHatchingTechnique().rotationAngleY,
								   0.f, 360.f, "%0.f degrees");

				// - SliderFloat (ángulo de rotación en eje Z)
				ImGui::SliderFloat("Rotation (Z Axis)##Skybox",
								   &skybox->getHatchingTechnique().rotationAngleZ,
								   0.f, 360.f, "%0.f degrees");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Hatching"))
				{
					currentElement->resetHatchingTechnique();
					skybox->resetHatchingTechnique();
				}

				break;

			case 6:
				resetAllRenderingModes();
				setRenderingMode("charcoal");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Escena
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Scene):");

				// - CheckBox (algoritmo Sobel filter)
				ImGui::Checkbox("Sobel filter##Charcoal",
								&quad->getCharcoalTechnique().sobelFilter);

				// - Configuración del algoritmo Sobel filter
				if (quad->getCharcoalTechnique().sobelFilter)
				{
					// - Color de los ejes
					ImGui::ColorEdit3("Edge color##Charcoal", 
									  &quad->getCharcoalTechnique().edgeColor[0]);

					// - Umbral
					ImGui::SliderFloat("Threshold##Charcoal",
									   &quad->getCharcoalTechnique().threshold,
									   0.01f, 0.1f, "%.3f");
				}

				// - Separador
				ImGui::Separator();

				// - Color del carboncillo
				ImGui::SliderFloat("Color multiplier##Charcoal",
								   &quad->getCharcoalTechnique().colorMultiplier,
								   0.f, 6.f, "RGB = %.0f * (0.1, 0.1, 0.1)");

				// - Factor de ruido
				ImGui::SliderFloat("Noise factor##Charcoal",
								   &quad->getCharcoalTechnique().noise,
								   0.f, 1.f, "%.2f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Charcoal"))
				{
					quad->resetCharcoalTechnique();
				}

				break;

			case 7:
				resetAllRenderingModes();
				setRenderingMode("goochShading");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Elemento
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Selected element):");

				// - Asignar anchura a los siguientes elementos de la GUI creados
				ImGui::PushItemWidth(ImGui::GetWindowSize().x / 4.85f);

				// - Color azulado
				ImGui::SliderFloat("###RKblue = (0, 0, B)##GoochShading",
								   &zero,
								   0.f, 0.f, "%.1f");

				// - Misma línea
				ImGui::SameLine();

				ImGui::SliderFloat("###GKblue = (0, 0, B)##GoochShading",
								   &zero,
								   0.f, 0.f, "%.1f");

				// - Misma línea
				ImGui::SameLine();

				ImGui::SliderFloat("Kblue = (0, 0, B)##GoochShading",
								   &currentElement->getGoochShadingTechnique().Kblue,
								   0.f, 1.f, "%.1f");

				// - Color amarillento
				ImGui::SliderFloat("###RKyellow = (R, G, 0)##GoochShading",
								   &currentElement->getGoochShadingTechnique().Kyellow,
								   0.f, 1.f, "%.1f");

				// - Misma línea
				ImGui::SameLine();

				ImGui::SliderFloat("###GKyellow = (R, G, 0)##GoochShading",
								   &currentElement->getGoochShadingTechnique().Kyellow,
								   0.f, 1.f, "%.1f");

				// - Misma línea
				ImGui::SameLine();

				ImGui::SliderFloat("Kyellow = (R, G, 0)##GoochShading",
								   &zero,
								   0.f, 0.f, "%.1f");

				// - Revertir anchura por defecto
				ImGui::PopItemWidth();

				// - Factor alfa
				ImGui::SliderFloat("Alpha (Kblue)##GoochShading",
								   &currentElement->getGoochShadingTechnique().alpha,
								   0.f, 1.f, "%.2f");

				// - Factor beta
				ImGui::SliderFloat("Beta (Kyellow)##GoochShading",
								   &currentElement->getGoochShadingTechnique().beta,
								   0.f, 1.f, "%.2f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##GoochShading"))
				{
					currentElement->resetGoochShadingTechnique();
				}

				break;

			case 8:
				resetAllRenderingModes();
				setRenderingMode("painterly");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Escena
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Scene):");

				// - Tamaño de la brocha
				ImGui::SliderInt("Brush size##Painterly",
								 &quad->getPainterlyTechnique().brushSize,
								 2, 7);

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##Painterly"))
				{
					quad->resetPainterlyTechnique();
				}

				break;

			case 9:
				resetAllRenderingModes();
				setRenderingMode("pixelArt");

				// - Añadir separador
				ImGui::Separator();

				// - Texto (parámetros de la técnica): Escena
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Technique parameters (Scene):");

				// - Número de pixels en horizontal
				ImGui::SliderFloat("Columns of pixels##PixelArt",
								   &quad->getPixelArtTechnique().numHorizontalPixels,
								   100.f, 400.f,
								   "%.0f");

				// - Número de pixels en vertical
				ImGui::SliderFloat("Rows of pixels##PixelArt",
								   &quad->getPixelArtTechnique().numVerticalPixels,
								   100.f, 400.f,
								   "%.0f");

				// - Añadir espacio
				ImGui::Spacing();

				// - Botón para restaurar a estado por defecto
				if (ImGui::Button("Default##PixelArt"))
				{
					quad->resetPixelArtTechnique();
				}

				break;
		}
	}
}

// - GUI de miscelánea
void Renderer::miscellaneousGUI()
{
	// - MISCELÁNEA
	if (ImGui::CollapsingHeader("Miscellaneous"))
	{
		// - Texto (medición de rendimiento)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Performance measurement:");

		// - Texto (ms/frame y FPS)
		ImGui::Text("Application average %.3f ms/frame (%.3f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// - Separador
		ImGui::Separator();

		// - Permitir seleccionar resolución de la captura de pantalla (no disponible para técnicas
		//   de post-procesamiento)
		if (enabledPostProcessing)
		{
			setScreenshotDimensions(viewportWidth, viewportHeight);
		}
		else
		{
			// - Texto (resoluciones disponibles)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Available screenshot resolutions:");

			// - Listado de resoluciones disponibles para la captura de pantalla
			static const char *resolutions[]
			{
				"Current window size",
				"720p (1280 x 720 pixels)",
				"1080p (1920 x 1080 pixels)",
				"1440p (2560 x 1440 pixels)",
				"4K (3840 x 2160 pixels)",
				"8K (7680 x 4320 pixels)",
			};

			// - Elemento seleccionado
			static int selectedItem = 0;

			// - Crear lista anidada
			ImGui::Combo("##ComboResolutions", &selectedItem, resolutions, IM_ARRAYSIZE(resolutions));

			// - Selector de resolución
			switch (selectedItem)
			{
				case 0:
					setScreenshotDimensions(viewportWidth, viewportHeight);
					break;

				case 1:
					setScreenshotDimensions(1280, 720);
					break;

				case 2:
					setScreenshotDimensions(1920, 1080);
					break;

				case 3:
					setScreenshotDimensions(2560, 1440);
					break;

				case 4:
					setScreenshotDimensions(3840, 2160);
					break;

				case 5:
					setScreenshotDimensions(7680, 4320);
					break;
				}

			// - Poner botón en la misma línea que el listado de resoluciones
			ImGui::SameLine();
		}

		// - Botón para hacer la captura de pantalla
		if (ImGui::Button("Take screenshot"))
		{
			takeScreenshot();
		}

		// - Notificar que la captura fue guardada
		if (screenshotTaken)
		{
			std::string message = screenshotName + std::string(" was saved successfully!");
			ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), message.c_str());
		}
	}
	else
	{
		screenshotTaken = false;
	}
}

/*
 **********************************************
			    FUNCIONALIDADES
 **********************************************
 */

// - Asignar dimensiones para la captura de pantalla
void Renderer::setScreenshotDimensions(unsigned int width, unsigned int height)
{
	screenshotWidth = width;
	screenshotHeight = height;
}

// - Tomar captura de pantalla
void Renderer::takeScreenshot()
{
	// - Si hay una técnica de post-procesamiento activa, se usa el tamaño del viewport
	if (enabledPostProcessing)
	{
		screenshotWidth = viewportWidth;
		screenshotHeight = viewportHeight;
	}

	// - Enlazar FBO
	fboScreenshot->bindFrameBuffer();

	// - Crear y enlazar buffers de color y profundidad
	fboScreenshot->bindRenderTexture();
	fboScreenshot->createRenderTexture(screenshotWidth, screenshotHeight);

	fboScreenshot->bindDepthBuffer();
	fboScreenshot->createDepthBuffer(screenshotWidth, screenshotHeight);

	// - Desenlazar buffers de color y profundidad
	fboScreenshot->unbindRenderTexture();
	fboScreenshot->unbindDepthBuffer();

	// - Enlazar buffers de color y profundidad al FBO
	fboScreenshot->attachRenderTexture();
	fboScreenshot->attachDepthBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Crear un viewport con las dimensiones para la captura de pantalla
	glViewport(0, 0, screenshotWidth, screenshotHeight);

	// - Recalcular aspecto y matriz de modelado, visión y proyección
	camera->setAspect(screenshotWidth, screenshotHeight);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();

	// - Dibujar la escena
	render();

	// - Tomar captura de pantalla utilizando las dimensiones del nuevo viewport
	screenshotName = "Screenshot-" + std::to_string(++screenshotCounter);
	fbo->takeScreenshot(screenshotName, screenshotWidth, screenshotHeight);

	// - Flag para indicar que se tomó la captura
	screenshotTaken = true;

	// - Desenlazar FBO (enlazar con el Window-System-Provided FrameBuffer)
	fboScreenshot->unbindFrameBuffer();

	// - Limpiar buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Restaurar viewport a sus dimensiones anteriores
	glViewport(0, 0, viewportWidth, viewportHeight);

	// - Recalcular aspect ratio y matriz de modelado, visión y proyección
	camera->setAspect(viewportWidth, viewportHeight);
	mvp = camera->getViewProjectionMatrix() * currentScene->getModelMatrix();
}