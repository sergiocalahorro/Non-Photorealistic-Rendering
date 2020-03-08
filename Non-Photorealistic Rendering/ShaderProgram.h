#pragma once
#include <iostream>
#include <GL\glew.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"

#include "Enumerations.h"

// - La clase ShaderProgram encapsula la carga, compilación, enlazado
//   y uso de un shader program. Los shader programs son piezas de código
//   que se cargan y ejecutan directamente en la GPU y permiten variar el
//   pipeline de rendering 3D según las necesidades del programador
class ShaderProgram
{
private:
	// - Identificador del shader program que permite referenciarlo en la GPU
	GLuint handler;

	// - Flag que indica si el shader program se ha enlazado correctamente
	bool linked;

	// - Cadena de caracteres que contiene el mensaje de error de la última operación sobre el shader
	std::string logString;

	// - Método privado que compila cada una de las partes del shader program
	GLuint compileShader(const char *filename, GLenum shaderType);

	// - Método auxiliar para comprobar si un archivo de recursos está presente
	bool fileExists(const std::string & fileName);

public:

	// - Constructor
	ShaderProgram();

	// - Destructor
	~ShaderProgram();

	// - Crea un shader program a partir del código fuente que se pasa en
	//   los archivos cuyo nombre genérico se pasa en el argumento filename.
	//   (Opcionalmente) Se puede crear un Geometry Shader object usando el flag de ShaderProgramFlags,
	//   estando desactivado por defecto
	GLuint createShaderProgram(const char *filename, ShaderProgramFlags flags = NO_GEOMETRY_SHADER);

	// - Activar el shader program
	bool use();

	// - Los siguientes métodos están sobrecargados. Permiten asignar parámetros de tipo uniform al shader
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