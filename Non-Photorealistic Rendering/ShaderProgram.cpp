#include "ShaderProgram.h"

// - Constructor
ShaderProgram::ShaderProgram()
{
	handler = 0;
	linked = false;
	logString = "";
}

// - Destructor
ShaderProgram::~ShaderProgram()
{
	// - Liberamos recursos en la GPU
	glDeleteProgram(handler);
	linked = false;
	logString = "";
}

// - Crea un shader program a partir del código fuente que se pasa en los archivos cuyo nombre genérico 
//   se pasa en el argumento filename. Este método, busca entre los recursos de la aplicación un archivo
//   cuyo nombre sea [filename]-vert.glsl y otro cuyo nombre sea [filename]-frag.glsl y con ellos crea el 
//   shader program.
//   (Opcionalmente) Se puede crear un Geometry Shader object usando el flag de ShaderProgramFlags,
//   estando desactivado por defecto. Si está activado, busca entre los recursos de la aplcación un archivo
//   [filename]-geom.glsl y crea el shader program junto a los vertex y fragment shader objects
GLuint ShaderProgram::createShaderProgram(const char *fileName, ShaderProgramFlags flags)
{
	// - Se crea el shader program y se almacena su identificador
	if (handler <= 0)
	{
		handler = glCreateProgram();
		if (handler == 0)
		{
			fprintf(stderr, "Cannot create shader program: %s.\n", fileName);
			return 0;
		}
	}

	// - Se carga y compila el vertex shader object
	char fileNameComplete[256];
	strcpy_s(fileNameComplete, fileName);
	strcat_s(fileNameComplete, "-vert.glsl");
	GLuint vertexShaderObject = compileShader(fileNameComplete, GL_VERTEX_SHADER);

	if (vertexShaderObject == 0)
	{
		return 0;
	}

	// - Asociar vertex shader object
	glAttachShader(handler, vertexShaderObject);

	// - Se carga y compila el fragment shader object
	strcpy_s(fileNameComplete, fileName);
	strcat_s(fileNameComplete, "-frag.glsl");
	GLuint fragmentShaderObject = compileShader(fileNameComplete, GL_FRAGMENT_SHADER);

	if (fragmentShaderObject == 0)
	{
		return 0;
	}

	// - Asociar fragment shader object
	glAttachShader(handler, fragmentShaderObject);

	// - Opcional: Se crea el Geometry Shader object, si el flag está activo, y se asocia al shader program
	//   si la compilación fue exitosa
	if (flags == GEOMETRY_SHADER)
	{
		// - Se carga y compila el geometry shader object
		strcpy_s(fileNameComplete, fileName);
		strcat_s(fileNameComplete, "-geom.glsl");
		GLuint geometryShaderObject = compileShader(fileNameComplete, GL_GEOMETRY_SHADER);

		if (geometryShaderObject == 0)
		{
			return 0;
		}

		// - Asociar geometry shader object
		glAttachShader(handler, geometryShaderObject);
	}

	// - Se enlaza el shader program y se comprueba si hay errores
	glLinkProgram(handler);

	GLint linkSuccess = 0;
	glGetProgramiv(handler, GL_LINK_STATUS, &linkSuccess);

	if (linkSuccess == GL_FALSE)
	{
		GLint logLen = 0;
		glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			char * cLogString = new char[logLen];
			GLint written = 0;
			glGetProgramInfoLog(handler, logLen, &written, cLogString);
			logString.assign(cLogString);
			delete[] cLogString;
			std::cout << "Cannot link shader " << fileName << std::endl << logString << std::endl;
		}
		return 0;
	}
	else
	{
		linked = true;
	}

	return handler;
}

// - Activar el shader program. A partir de ese momento y hasta que no se active un shader program distinto,
//   las órdenes de dibujo se procesarán siguiendo las instrucciones de este programa
bool ShaderProgram::use()
{
	// - Antes de activar un shader program para su uso, hay que comprobar
	// si se ha creado bien y se ha enlazado bien
	if ((handler > 0) && (linked)) 
	{
		glUseProgram(handler);
		return true;
	}
	else 
	{
		std::cout << "Cannot use shader program" << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (int)
bool ShaderProgram::setUniform(std::string name, GLint value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0) 
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo GLint
		glUniform1i(location, value);
		return true;
	}
	else 
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (float)
bool ShaderProgram::setUniform(std::string name, GLfloat value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0) 
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo GLfloat
		glUniform1f(location, value);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (bool)
bool ShaderProgram::setUniform(std::string name, GLboolean value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0)
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo GLfloat
		glUniform1i(location, value);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (mat2)
bool ShaderProgram::setUniform(std::string name, glm::mat2 value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0)
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo
		//   mat2 con valores GLfloat y expresado como un array
		glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (mat3)
bool ShaderProgram::setUniform(std::string name, glm::mat3 value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0)
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo
		//   mat3 con valores GLfloat y expresado como un array
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (mat4)
bool ShaderProgram::setUniform(std::string name, glm::mat4 value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0) 
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo
		//   mat4 con valores GLfloat y expresado como un array
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
		return true;
	}
	else 
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (vec2)
bool ShaderProgram::setUniform(std::string name, glm::vec2 value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0)
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo
		//   vec3 con valores GLfloat y expresado como un array
		glUniform3fv(location, 1, &value[0]);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Permite asignar parámetros de tipo uniform al shader (vec3)
bool ShaderProgram::setUniform(std::string name, glm::vec3 value)
{
	// - Para asignar valor a un uniform, primero hay que buscar si en el shader
	//   program existe alguna variable de tipo uniform cuyo nombre coincida con
	//   el que pasamos como argumento
	GLint location = glGetUniformLocation(handler, name.c_str());

	// - Si location es un valor positivo, es que existe el uniform y podemos
	//   asignarlo
	if (location >= 0)
	{
		// - Aquí usamos la función glUniform que recibe un argumento de tipo
		//   vec3 con valores GLfloat y expresado como un array
		glUniform3fv(location, 1, &value[0]);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << name << std::endl;
		return false;
	}
}

// - Asignar subrutina
bool ShaderProgram::setSubroutine(std::string uniformName, std::string subroutineName,
								  GLenum shaderType, GLsizei count)
{
	// - Primero hay que buscar si en el shader program existe el uniform de la subrutina
	//   cuyo nombre coincida con el que se pasa como argumento
	GLint location = glGetSubroutineUniformLocation(handler, shaderType, uniformName.c_str());

	// - Si location es un valor positivo, es que existe la subrutina y podemos asignarlo
	if (location >= 0)
	{
		// - Averiguar el identificador de la subroutine que nos interesa seleccionar para que se ejecute
		GLuint index = glGetSubroutineIndex(handler, shaderType, subroutineName.c_str());

		// - Activar la subrutina
		glUniformSubroutinesuiv(shaderType, count, &index);
		return true;
	}
	else
	{
		std::cout << "Cannot find localization for: " << uniformName << std::endl;
		return false;
	}
}

// - Método privado que compila cada una de las partes del shader program 
GLuint ShaderProgram::compileShader(const char *filename, GLenum shaderType)
{
	// - Comprobamos si en la solución existe algún archivo de recursos con el
	//   nombre que se pasa como argumento
	if (!fileExists(filename)) 
	{
		fprintf(stderr, "Shader source file %s not found.\n", filename);
		return 0;
	}

	// - Si existe se lee en una cadena de caracteres que contiene el listado
	//   completo del shader source
	std::ifstream shaderSourceFile;
	shaderSourceFile.open(filename);

	if (!shaderSourceFile)
	{
		fprintf(stderr, "Cannot open shader source file.\n");
		return 0;
	}

	std::stringstream shaderSourceStream;
	shaderSourceStream << shaderSourceFile.rdbuf();
	std::string shaderSourceString = shaderSourceStream.str();
	shaderSourceFile.close();

	// - Creamos un shader object para ese archivo que se ha leído
	GLuint shaderHandler = glCreateShader(shaderType);

	if (shaderHandler == 0)
	{
		fprintf(stderr, "Cannot create shader object.\n");
		return 0;
	}

	// - Le asignamos el código fuente leído y lo compilamos
	const char * shaderSourceCString = shaderSourceString.c_str();
	glShaderSource(shaderHandler, 1, &shaderSourceCString, NULL);
	glCompileShader(shaderHandler);

	// - Se comprueba si la compilación se ha realizado con éxito
	GLint compileResult;
	glGetShaderiv(shaderHandler, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE) 
	{
		GLint logLen = 0;
		logString = "";
		glGetShaderiv(shaderHandler, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) 
		{
			char * cLogString = new char[logLen];
			GLint written = 0;
			glGetShaderInfoLog(shaderHandler, logLen, &written, cLogString);
			logString.assign(cLogString);
			delete[] cLogString;
			std::cout << "Cannot compile shader " << shaderType << std::endl
					  << logString << std::endl;
		}
	}

	return shaderHandler;
}

// - Método auxiliar para comprobar si un archivo de recursos está presente
bool ShaderProgram::fileExists(const std::string & fileName)
{
	struct stat info;
	int ret = -1;
	ret = stat(fileName.c_str(), &info);
	return 0 == ret;
}