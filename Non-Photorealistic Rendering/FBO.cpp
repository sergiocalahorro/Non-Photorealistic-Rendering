#include "FBO.h"
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <vector>

// - Constructor
FBO::FBO()
{
	fboHandle = 0;
	renderTex = 0;
	depthTex = 0;

	glGenFramebuffers(1, &fboHandle);
	glGenTextures(1, &renderTex);
	glGenRenderbuffers(1, &depthTex);
}

// - Destructor
FBO::~FBO()
{
	glDeleteFramebuffers(1, &fboHandle);
	glDeleteTextures(1, &renderTex);
	glDeleteRenderbuffers(1, &depthTex);
}

// - Enlazar el Frame Buffer
void FBO::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
}

// - Desenlazar el Frame Buffer (enlazar con el Window-System-Provided Frame Buffer)
void FBO::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// - Activar textura de unidad
void FBO::activeTextureUnit(unsigned int unit)
{
	if (unit >= 0 && unit <= 47)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
	}
}

// - Crear textura de color
void FBO::createRenderTexture(unsigned int width, unsigned int height)
{
	// - Asignar parámetros a la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// - Crear textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

// - Enlazar textura de color
void FBO::bindRenderTexture()
{
	// - Enlazar textura
	glBindTexture(GL_TEXTURE_2D, renderTex);
}

// - Desenlazar unidad de textura de color
void FBO::unbindRenderTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// - Asociar buffer de color al FBO
void FBO::attachRenderTexture()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);
}

// - Crear buffer de profundidad
void FBO::createDepthBuffer(unsigned int width, unsigned int height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
}

// - Enlazar buffer de profundidad
void FBO::bindDepthBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, depthTex);
}

// - Desenlazar unidad de textura de profundidad
void FBO::unbindDepthBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

// - Asociar buffer de profundidad al FBO
void FBO::attachDepthBuffer()
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTex);
}

// - Comprobar estado del FBO
void FBO::checkStatus()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cout << "ERROR al crear FBO" << std::endl;
	}
}

// - Tomar captura de pantalla
void FBO::takeScreenshot(std::string &filename, unsigned int width, unsigned int height)
{
	// - Reservar memoria para almacenar los bytes de la imagen
	GLubyte *image = (GLubyte *) malloc(sizeof(GLubyte) * width * height * 4);

	// - Leer pixels del Framebuffer
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// - Si los pixels se almacenaron correctamente, se guarda la imagen en disco
	if (image != nullptr)
	{
		// - Inicializar DevIL, generar y enlazar imagen
		ilInit();
		ILuint imageId = ilGenImage();
		ilBindImage(imageId);
		
		// - Definir imagen
		ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, image);
		ilSetData(image);

		// - Permitir sobreescribir archivo
		ilEnable(IL_FILE_OVERWRITE);

		// - Añadir extensión .png si no se añadió
		std::string filenameExtension = filename;
		if (filename.substr(filename.find_last_of(".") + 1) != "png")
		{
			filenameExtension += ".png";
		}
		
		// - Guardar imagen
		ilSaveImage(filenameExtension.c_str());

		// - Liberar recursos
		ilDeleteImage(imageId);
		delete image;
	}
}