#include "Texture.h"
#include "lodepng.h"

// - Constructor por defecto
Texture::Texture()
{
	texture = 0;
	glGenTextures(1, &texture);
}

// - Constructor
Texture::Texture(std::string type, std::string path)
{
	texture = 0;
	glGenTextures(1, &texture);

	this->type = type;
	this->path = path;
}

// - Destructor
Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

// - Cargar imagen de disco
void Texture::loadImage(const char *path, bool invertImage)
{
	/** Carga un png de disco https://lodev.org/lodepng/ */
	unsigned error = lodepng::decode(image, width, height, path);

	if (error)
	{
		// - Error en la carga del fichero
		std::cout << path << " cannot be loaded" << std::endl;
		return;
	}

	// - La textura se carga del revés, así que hay que darle la vuelta
	if (!invertImage)
	{
		unsigned char *imgPtr = &image[0];
		int numColorComponents = 4;
		int widthInChar = width * 4;
		unsigned char* top = nullptr;
		unsigned char* bot = nullptr;
		unsigned char temp = 0;

		for (int i = 0; i < height / 2; i++)
		{
			top = imgPtr + i * widthInChar;
			bot = imgPtr + (height - i - 1) * widthInChar;
			for (int j = 0; j < widthInChar; j++)
			{
				temp = *top;
				*top = *bot;
				*bot = temp;
				++top;
				++bot;
			}
		}
	}
}


// - Cargar imagen de disco (directorio)
void Texture::loadImage(const char *path, const std::string &directory, bool invertImage)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	/** Carga un png de disco https://lodev.org/lodepng/ */
	unsigned error = lodepng::decode(image, width, height, filename);

	if (error)
	{
		// - Error en la carga del fichero
		std::cout << filename << " cannot be loaded" << std::endl;
		return;
	}

	// - La textura se carga del revés, así que hay que darle la vuelta
	if (!invertImage)
	{
		unsigned char *imgPtr = &image[0];
		int numColorComponents = 4;
		int widthInChar = width * 4;
		unsigned char* top = nullptr;
		unsigned char* bot = nullptr;
		unsigned char temp = 0;

		for (int i = 0; i < height / 2; i++)
		{
			top = imgPtr + i * widthInChar;
			bot = imgPtr + (height - i - 1) * widthInChar;
			for (int j = 0; j < widthInChar; j++)
			{
				temp = *top;
				*top = *bot;
				*bot = temp;
				++top;
				++bot;
			}
		}
	}
}

// - Definir una textura dados sus parámetros
void Texture::defineTexture(GLenum target,
							GLenum min, GLenum mag, GLint paramMinMag,
							GLenum wrapS, GLenum wrapT, GLint paramWrap,
							int level, GLint internalFormat, GLsizei width, GLsizei height,
							GLint border, GLenum format, GLenum type, std::vector<unsigned char> image)
{
	// - Definir parámetros de la textura
	glTexParameteri(target, min, paramMinMag);
	glTexParameteri(target, mag, paramMinMag);
	glTexParameteri(target, wrapS, paramWrap);
	glTexParameteri(target, wrapT, paramWrap);

	glTexImage2D(target, level, internalFormat, width, height, border, format, type, image.data());

	// - Generar Mipmap automáticamente
	glGenerateMipmap(target);
}

// - Definir una textura dados sus parámetros (CubeMap)
void Texture::defineTexture(GLenum target, GLenum targetImage,
							GLenum min, GLenum mag, GLint paramMinMag,
							GLenum wrapS, GLenum wrapT, GLenum wrapR, GLint paramWrap,
							int level, GLint internalFormat, GLsizei width, GLsizei height,
							GLint border, GLenum format, GLenum type, std::vector<unsigned char> image)
{
	// - Definir parámetros de la textura
	glTexParameteri(target, min, paramMinMag);
	glTexParameteri(target, mag, paramMinMag);
	glTexParameteri(target, wrapS, paramWrap);
	glTexParameteri(target, wrapT, paramWrap);
	glTexParameteri(target, wrapR, paramWrap);

	glTexImage2D(targetImage, level, internalFormat, width, height, border, format, type, image.data());
}


// - Enlazar unidad de textura
void Texture::bindTexture(GLenum target, unsigned int unit)
{
	if (unit >= 0 && unit <= 47)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, texture);
	}
}

// - Desenlazar unidad de textura
void Texture::unbindTexture(GLenum target)
{
	glBindTexture(target, 0);
}

// - Obtener imagen (en vector de char)
std::vector<unsigned char> Texture::getImage()
{
	return image;
}

// - Obtener anchura de la imagen en pixels
unsigned Texture::getWidth()
{
	return width;
}

// - Obtener altura de la imagen en pixels
unsigned Texture::getHeight()
{
	return height;
}

// - Obtener tipo de la textura
std::string Texture::getType() 
{
	return type;
}

// - Obtener ruta del fichero de la textura
std::string Texture::getPath() 
{
	return path;
}

// - Asignar tipo de la textura
void Texture::setType(std::string type)
{
	this->type = type;
}

// - Asignar ruta del fichero de la textura
void Texture::setPath(std::string path)
{
	this->path = path;
}