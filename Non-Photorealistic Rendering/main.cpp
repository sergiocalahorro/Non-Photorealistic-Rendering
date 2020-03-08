#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>
// - IMPORTANTE: El include de Glew debe llamarse siempre ANTES de llamar al de GLFW
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

// - Dimensiones de la ventana
const int WIDTH = 1280;
const int HEIGHT = 720;
const std::string WINDOW_TITLE = "Non-Photorealistic Rendering";

// - Variables para almacenar las dimensiones actuales de la ventana (actualizadas al redimensionar)
int actualWidth;
int actualHeight;

// - Diferencia de tiempo entre cada frame
const float DELTA_TIME = 1.f / 60.f;

// - Velocidades aplicadas a cada tipo de acci�n de la c�mara
const float MOVE_SPEED = 20.f;
const float ORBIT_SPEED = 80.f;
const float PAN_SPEED = 50.f;
const float ZOOM_SPEED = 50.f;

// - Esta funci�n callback ser� llamada cada vez que el �rea de dibujo
//   OpenGL deba ser redibujada
void window_refresh_callback(GLFWwindow *window) 
{
	// - Para llamar al m�todo que refresca el �rea de dibujo OpenGL primero hay que
    //   acceder al singleton. Esto se hace a trav�s del m�todo de clase getInstance() 
	//   que devuelve un puntero al singleton
	Renderer::getInstance()->windowRefreshCallback();

	// - ImGui: Preparar nuevo frame de la interfaz gr�fica para ser renderizada en la ventana de GLFW
	ImGui_ImplGlfwGL3_NewFrame();

	// - Dibujar la escena
	Renderer::getInstance()->render();

	// - Renderizar GUI principal de la aplicaci�n
	Renderer::getInstance()->renderMainGUI();

	// - ImGui: Dibujar interfaz gr�fica en la ventana de GLFW
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
	//   intercambia el buffer back (que se ha estado dibujando) por el
	//   que se mostraba hasta ahora front
	glfwSwapBuffers(window);
}

// - Esta funci�n callback ser� llamada cada vez que se cambie el tama�o
//   del �rea de dibujo OpenGL
void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
	// - Para llamar al m�todo que cambia el tama�o del �rea de dibujo OpenGL primero
	//   hay que acceder al singleton. Esto se hace a trav�s del m�todo de clase
	//   getInstance() que devuelve un puntero al singleton
	Renderer::getInstance()->frameBufferSizeCallback(width, height);

	glfwGetFramebufferSize(window, &actualWidth, &actualHeight);
}

// - Esta funci�n callback ser� llamada cada vez que se pulse una tecla
//   dirigida al �rea de dibujo OpenGL
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) 
{
	// - Para llamar al m�todo que responde al evento de pulsar una tecla sobre 
	//   el �rea de dibujo OpenGL primero hay que acceder al singleton. Esto se hace a
	//   trav�s del m�todo de clase getInstance() que devuelve un puntero al singleton
	Renderer::getInstance()->keyCallback();

	// - Cerrar la ventana al pulsar 'ESC'
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	// - Visualizar la escena 1, al pulsar la tecla '1'
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->setupScene(0);
		window_refresh_callback(window);
	}

	// - Visualizar la escena 1, al pulsar la tecla '2'
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->setupScene(1);
		window_refresh_callback(window);
	}

	// - Visualizar la escena 3, al pulsar la tecla '3'
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->setupScene(2);
		window_refresh_callback(window);
	}

	// - Dibujar la escena, de forma realista, al pulsar la tecla '0' del teclado num�rico
	if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("realistic");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, de forma monocrom�tica, al pulsar la tecla '1' del teclado num�rico
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("monochrome");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Cel-Shading, al pulsar la tecla '2' del teclado num�rico
	if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("celShading");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Halftone, al pulsar la tecla '3' del teclado num�rico
	if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("halftone");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Dithering, al pulsar la tecla '4' del teclado num�rico
	if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("dithering");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Hatching, al pulsar la tecla '5' del teclado num�rico
	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("hatching");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Charcoal, al pulsar la tecla '6' del teclado num�rico
	if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("charcoal");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Gooch Shading, al pulsar la tecla '7' del teclado num�rico
	if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("goochShading");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Painterly, al pulsar la tecla '8' del teclado num�rico
	if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("painterly");
		window_refresh_callback(window);
	}

	// - Dibujar la escena, utilizando la t�cnica Pixel Art, al pulsar la tecla '9' del teclado num�rico
	if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS)
	{
		Renderer::getInstance()->resetAllRenderingModes();
		Renderer::getInstance()->setRenderingMode("pixelArt");
		window_refresh_callback(window);
	}

	// - Dibujar contorno b�sico alrededor de los objetos pulsando la tecla 'B'
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		Renderer::getInstance()->toggleBasicOutline();
		window_refresh_callback(window);
	}

	// - Dibujar contorno avanzado alrededor de los objetos pulsando la tecla 'L'
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		Renderer::getInstance()->toggleAdvancedOutline();
		window_refresh_callback(window);
	}

	// - Mostrar/Ocultar GUI
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		Renderer::getInstance()->toggleWindow();
		window_refresh_callback(window);
	}

	// - Tomar captura de pantalla
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		Renderer::getInstance()->takeScreenshot();
		window_refresh_callback(window);
	}

	// - Pan: Rotar objetivo de la c�mara a la izquierda
	if (key == GLFW_KEY_LEFT)
	{
		Renderer::getInstance()->cameraPanMovement(PAN_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Pan: Rotar objetivo de la c�mara a la derecha
	if (key == GLFW_KEY_RIGHT)
	{
		Renderer::getInstance()->cameraPanMovement(-PAN_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Tilt: Rotar objetivo de la c�mara hacia arriba
	if (key == GLFW_KEY_UP)
	{
		Renderer::getInstance()->cameraTiltMovement(PAN_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Tilt: Rotar objetivo de la c�mara hacia abajo
	if (key == GLFW_KEY_DOWN)
	{
		Renderer::getInstance()->cameraTiltMovement(-PAN_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Truck: Mover c�mara a la izquierda
	if (key == GLFW_KEY_A)
	{
		Renderer::getInstance()->cameraTruckMovement(-MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Truck: Mover c�mara a la derecha
	if (key == GLFW_KEY_D)
	{
		Renderer::getInstance()->cameraTruckMovement(MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Boom: Mover c�mara hacia arriba
	if (key == GLFW_KEY_E)
	{
		Renderer::getInstance()->cameraBoomMovement(MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Crane (Boom inverso): Mover c�mara hacia abajo
	if (key == GLFW_KEY_Q)
	{
		Renderer::getInstance()->cameraBoomMovement(-MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Dolly: Mover c�mara hacia adelante
	if (key == GLFW_KEY_W)
	{
		Renderer::getInstance()->cameraDollyMovement(MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Dolly: Mover c�mara hacia atr�s
	if (key == GLFW_KEY_S)
	{
		Renderer::getInstance()->cameraDollyMovement(-MOVE_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Orbit: Orbitar c�mara alrededor del punto de inter�s
	if (key == GLFW_KEY_O)
	{
		Renderer::getInstance()->cameraOrbitMovement(ORBIT_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Hacer zoom 
	if (key == GLFW_KEY_KP_ADD)
	{
		Renderer::getInstance()->cameraZoom(ZOOM_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Quitar zoom
	if (key == GLFW_KEY_KP_SUBTRACT)
	{
		Renderer::getInstance()->cameraZoom(-ZOOM_SPEED * DELTA_TIME);
		window_refresh_callback(window);
	}

	// - Resetear c�mara
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		Renderer::getInstance()->cameraReset();
		window_refresh_callback(window);
	}
}

// - Esta funci�n callback ser� llamada cada vez que se pulse alg�n bot�n
//   del rat�n sobre el �rea de dibujo OpenGL
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) 
{
	// - Para llamar al m�todo que responde al evento de pulsar un bot�n del rat�n sobre
	//   el �rea de dibujo OpenGL primero hay que acceder al singleton. Esto se hace a
	//   trav�s del m�todo de clase getInstance() que devuelve un puntero al singleton
	Renderer::getInstance()->mouseButtonCallback();

	if (action == GLFW_PRESS) 
	{
		std::cout << "Pulsado el boton del raton: " << button << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		std::cout << "Soltado el boton del raton: " << button << std::endl;
	}
}

// - Esta funci�n callback ser� llamada cada vez que se mueva la rueda
//   del rat�n sobre el �rea de dibujo OpenGL
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) 
{
	// - Para llamar al m�todo que responde al evento de mover la rueda del rat�n sobre
	//	 el �rea de dibujo OpenGL primero hay que acceder al singleton. Esto se hace a
	//   trav�s del m�todo de clase getInstance() que devuelve un puntero al singleton
	Renderer::getInstance()->scrollCallback(xoffset, yoffset);

	// - Permitir hacer zoom con la rueda del rat�n cuando el cursor no est� sobre la GUI
	if (!Renderer::getInstance()->isScrollEnabled())
	{
		// - Zoom de la c�mara
		Renderer::getInstance()->cameraZoom((float) yoffset * 2.f);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float) xoffset;
		io.MouseWheel += (float) yoffset;
	}

	window_refresh_callback(window);
}

// - Funci�n principal
int main()
{
	std::cout << "Starting application..." << std::endl;

	// - Inicializar GLFW. Es un proceso que s�lo debe realizarse una vez
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// - Definimos las caracter�sticas que queremos que tenga nuestro contexto gr�fico
	//   OpenGL. Por ejemplo, el n�mero de muestras o modo Core Profile

	// - Activa antialiasing con 4 muestras
	glfwWindowHint(GLFW_SAMPLES, 4); 

	// - Esta y las siguientes activan un contexto OpenGL Core Profile 4.1
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// - Definimos la ventana de la aplicaci�n y la creamos
	GLFWwindow *window;

	// - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa,
	//   sin compartir recursos con otras ventanas
	window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);

	// - Almacenar las dimensiones iniciales de la ventana
	actualWidth = WIDTH;
	actualHeight = HEIGHT;

	// - Comprobamos si la creaci�n de la ventana ha tenido �xito
	if (window == nullptr)
	{
		std::cout << "Failed to open GLFW window" << std::endl;

		// - Liberamos los recursos que ocupaba GLFW
		glfwTerminate(); 
		return -2;
	}

	// - Hace que la ventana que se pasa como argumento use el contexto
	//   de GLFW cuyas caracter�sticas se han definido previamente
	glfwMakeContextCurrent(window);

	// - Ahora inicializamos GLEW
	//   IMPORTANTE: Glew debe inicializarse siempre DESPU�S de que se haya
	//   inicializado y creado GLFW
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;

		// - Liberamos los recursos que ocupaba GLFW
		glfwTerminate();
		return -3;
	}

	// - Registramos los callbacks que responder�n a los eventos principales
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// - Preparar la escena para su visualizaci�n (activar par�metros globales de OpenGL y
	//   creaci�n de los elementos de la escena)
	Renderer::getInstance()->prepareOpenGL(WIDTH, HEIGHT);

	// - ImGui: Preparar el contexto de ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	// - ImGui: Iniciar interfaz gr�fica en la ventana del contexto creado en GLFW
	ImGui_ImplGlfwGL3_Init(window, false);

	// - ImGui: Elegir estilo de colores de la interfaz (Modo oscuro)
	ImGui::StyleColorsDark();

	// - Ciclo de eventos de la aplicaci�n. La condici�n de parada es que la
	//   ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
	//   bot�n de cerrar la ventana (la X)
	while (!glfwWindowShouldClose(window))
	{
		// - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
		//   teclas o de rat�n, etc.
		glfwPollEvents();

		// - Limpiar buffers de color y profundidad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// - ImGui: Preparar nuevo frame de la interfaz gr�fica
		ImGui_ImplGlfwGL3_NewFrame();

		// - Dibujar la escena
		Renderer::getInstance()->render();

		// - Renderizar GUI principal de la aplicaci�n
		Renderer::getInstance()->renderMainGUI();

		// - ImGui: Dibujar interfaz gr�fica en la ventana de GLFW
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
		//   intercambia el buffer back (que se ha estado dibujando) por el
		//   que se mostraba hasta ahora front
		glfwSwapBuffers(window);
	}

	// - Una vez terminado el ciclo de eventos, liberar recursos, etc.
	std::cout << "Finishing application..." << std::endl;

	// - ImGui: Destruir contexto y liberar recursos
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	// - Cerramos y destruimos la ventana de la aplicaci�n
	glfwDestroyWindow(window); 

	// - Liberamos los recursos que ocupaba GLFW
	glfwTerminate(); 

	// - Esta llamada es para impedir que la consola se cierre inmediatamente tras la
	//   ejecuci�n y poder leer los mensajes. Se puede usar tambi�n getChar();
	system("pause");
}