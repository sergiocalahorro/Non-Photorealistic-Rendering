#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <string>

class Texture
{
private:
	// - Identificador de textura
	GLuint texture;

	// - Imagen y sus parámetros
	std::vector<unsigned char> image;
	unsigned width;
	unsigned height;

	// - Tipo de la textura
	std::string type;

	// - Ruta del fichero que contiene la imagen
	std::string path;

public:
	// - Constructor por defecto
	Texture();

	// - Constructor
	Texture(std::string type, std::string path);

	// - Destructor
	~Texture();

	// - Cargar imagen de disco
	void loadImage(const char *path, bool invertImage = false);

	// - Cargar imagen de disco (con directorio)
	void loadImage(const char *path, const std::string &directory, bool invertImage = false);

	// - Definir una textura dados sus parámetros
	void defineTexture(GLenum target,
					   GLenum min, GLenum mag, GLint paramMinMag,
					   GLenum wrapS, GLenum wrapT, GLint paramWrap,
					   int level, GLint internalFormat, GLsizei width, GLsizei height,
					   GLint border, GLenum format, GLenum type, std::vector<unsigned char> image);

	// - Definir una textura dados sus parámetros (CubeMap)
	void defineTexture(GLenum target, GLenum targetImage,
					   GLenum min, GLenum mag, GLint paramMinMag,
					   GLenum wrapS, GLenum wrapT, GLenum wrapR, GLint paramWrap,
					   int level, GLint internalFormat, GLsizei width, GLsizei height,
					   GLint border, GLenum format, GLenum type, std::vector<unsigned char> image);

	// - Enlazar unidad de textura
	void bindTexture(GLenum target, unsigned int unit);

	// - Desenlazar unidad de textura
	void unbindTexture(GLenum target);

	// - Getters
	std::vector<unsigned char> getImage();
	unsigned getWidth();
	unsigned getHeight();

	std::string getType();
	std::string getPath();

	// - Setters
	void setType(std::string type);
	void setPath(std::string path);
};