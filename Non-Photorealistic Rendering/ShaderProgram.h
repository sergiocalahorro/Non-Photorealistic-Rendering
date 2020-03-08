#pragma once
#include <iostream>
#include <GL\glew.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"

#include "Enumerations.h"

// - La clase ShaderProgram encapsula la carga, compilaci�n, enlazado
//   y uso de un shader program. Los shader programs son piezas de c�digo
//   que se cargan y ejecutan directamente en la GPU y permiten variar el
//   pipeline de rendering 3D seg�n las necesidades del programador
class ShaderProgram
{
private:
	// - Identificador del shader program que permite referenciarlo en la GPU
	GLuint handler;

	// - Flag que indica si el shader program se ha enlazado correctamente
	bool linked;

	// - Cadena de caracteres que contiene el mensaje de error de la �ltima operaci�n sobre el shader
	std::string logString;

	// - M�todo privado que compila cada una de las partes del shader program
	GLuint compileShader(const char *filename, GLenum shaderType);

	// - M�todo auxiliar para comprobar si un archivo de recursos est� presente
	bool fileExists(const std::string & fileName);

public:

	// - Constructor
	ShaderProgram();

	// - Destructor
	~ShaderProgram();

	// - Crea un shader program a partir del c�digo fuente que se pasa en
	//   los archivos cuyo nombre gen�rico se pasa en el argumento filename.
	//   (Opcionalmente) Se puede crear un Geometry Shader object usando el flag de ShaderProgramFlags,
	//   estando desactivado por defecto
	GLuint createShaderProgram(const char *filename, ShaderProgramFlags flags = NO_GEOMETRY_SHADER);

	// - Activar el shader program
	bool use();

	// - Los siguientes m�todos est�n sobrecargados. Permiten asignar par�metros de tipo uniform al shader
	bool setUniform(std::string name, GLfloat value);
	bool setUniform(std::string name, GLint value);
	bool setUniform(std::string name, GLboolean value);
	bool setUniform(std::string name, glm::mat2 value);
	bool setUniform(std::string name, glm::mat3 value);
	bool setUniform(std::string name, glm::mat4 value);
	bool setUniform(std::string name, glm::vec2 value);
	bool setUniform(std::string name, glm::vec3 value);

	// - Asignar subrutina
	bool setSubroutine(std::string uniformName, std::string subroutineName, GLenum shaderType, GLsizei count);
};