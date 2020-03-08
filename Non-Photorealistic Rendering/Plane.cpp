#include "Plane.h"

// - Constructor
Plane::Plane(int width, int height, int tilingHorizontal, int tilingVertical)
{
	// - Tamaño de cada "celda" del plano
	float sizeX = (float) width / tilingHorizontal;
	float sizeZ = (float) height / tilingVertical;

	// - Geometría 
	for (int j = 0; j <= tilingVertical; j++)
	{
		for (int i = 0; i <= tilingHorizontal; i++)
		{
			// - Vértice (posicion y normal). El origen de coordenadas se corresponderá con
			//   el centro del plano
			PosNorm vertice;
			vertice.position = glm::vec3((width / 2) - (i * sizeX), 0, (height / 2) - (j * sizeZ));
			vertice.normal = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
			vertices.push_back(vertice);

			// - Coordenadas de textura
			float s = float(i) / tilingHorizontal;
			float t = float(j) / tilingVertical;
			glm::vec2 texCoord(s, t);
			textureCoords.push_back(texCoord);

			// - Tangente
			glm::vec3 tangent(1.f, 0.f, 0.f);
			tangents.push_back(tangent);
		}
	}

	// - Topología (malla de triángulos)

	// - En sentido invertido para tener las caras visibles en la parte de arriba
	for (int i = tilingHorizontal - 1; i >= 0; i--)
	{
		for (int j = tilingVertical; j >= 0; j--)
		{
			topology.push_back(j * (tilingHorizontal + 1) + i);
			topology.push_back(j * (tilingHorizontal + 1) + (i + 1));
		}
		// - Reinicio de primitiva
		topology.push_back(0xFFFFFFFF);
	}

	// - Crear VAO
	vao = new VAO();

	// - Crear VBOs
	vao->fillVBO(vertices, textureCoords, tangents);

	// - Material
	material = new Material();
	material->setKa(glm::vec3(0.3f, 0.3f, 0.3f));
	material->setKd(glm::vec3(0.f, 0.f, 0.f));
	material->setKs(glm::vec3(0.3f, 0.3f, 0.3f));
	material->setShininess(8.f);

	// - Es un plano
	isPlane = true;
}

// - Destructor
Plane::~Plane()
{
	vertices.clear();
	textureCoords.clear();
	tangents.clear();
	topology.clear();
}

// - Cargar textura
void Plane::loadTexture(std::string filename)
{
	texture = new Texture();
	texture->loadImage(filename.c_str());
	texture->bindTexture(GL_TEXTURE_2D, 0);
	texture->defineTexture(GL_TEXTURE_2D,
						   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
						   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
						   0, GL_RGBA, texture->getWidth(), texture->getHeight(),
						   0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImage());
}

// - Asignar texturas de Hatching
void Plane::setHatchingTextures(std::string dark, std::string bright)
{
	hatchDark = new Texture();
	hatchDark->loadImage(dark.c_str());
	hatchDark->bindTexture(GL_TEXTURE_2D, 1);
	hatchDark->defineTexture(GL_TEXTURE_2D,
							 GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							 GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							 0, GL_RGBA, hatchDark->getWidth(), hatchDark->getHeight(),
							 0, GL_RGBA, GL_UNSIGNED_BYTE, hatchDark->getImage());

	hatchBright = new Texture();
	hatchBright->loadImage(bright.c_str());
	hatchBright->bindTexture(GL_TEXTURE_2D, 2);
	hatchBright->defineTexture(GL_TEXTURE_2D,
							   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							   0, GL_RGBA, hatchBright->getWidth(), hatchBright->getHeight(),
							   0, GL_RGBA, GL_UNSIGNED_BYTE, hatchBright->getImage());
}

// - Dibujado del plano de forma realista
void Plane::drawRealistic(ShaderProgram &shader, glm::mat4 mModel,
						  glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", material->getKa());
	shader.setUniform("KdMaterial", material->getKd());
	shader.setUniform("KsMaterial", material->getKs());
	shader.setUniform("shininess", material->getShininess());

	// - Asignar el sampleador de textura correspondiente
	shader.setUniform("TexSamplerDiffuse", 0);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	texture->bindTexture(GL_TEXTURE_2D, 0);

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del plano de forma monocromática (material)
void Plane::drawMonochrome(ShaderProgram &shader, glm::mat4 mModel,
						   glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", monochrome.material.getKa());
	shader.setUniform("KdMaterial", monochrome.material.getKd());
	shader.setUniform("KsMaterial", monochrome.material.getKs());
	shader.setUniform("shininess", monochrome.material.getShininess());

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del plano utilizando la técnica Cel-Shading
void Plane::drawCelShading(ShaderProgram &shader, glm::mat4 mModel, 
						   glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", material->getKa());
	shader.setUniform("KdMaterial", material->getKd());
	shader.setUniform("KsMaterial", material->getKs());
	shader.setUniform("shininess", material->getShininess());
	shader.setUniform("tones", celShading.tones);
	shader.setUniform("silhouettingFactor", celShading.silhouettingFactor);

	// - Asignar el sampleador de textura correspondiente
	shader.setUniform("TexSamplerDiffuse", 0);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	texture->bindTexture(GL_TEXTURE_2D, 0);

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del plano con la técnica hatching
void Plane::drawHatching(ShaderProgram &shader, glm::mat4 mModel,
						 glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", glm::vec3(1.0));
	shader.setUniform("KdMaterial", glm::vec3(1.0));
	shader.setUniform("KsMaterial", glm::vec3(1.0));
	shader.setUniform("shininess", 8.f);
	shader.setUniform("density", hatching.density);

	// - Ángulos de rotación
	float angleXRad = hatching.rotationAngleX;
	float angleYRad = hatching.rotationAngleY;

	// - Matrices de rotación
	glm::mat2 rotationXMatrix(glm::cos(angleXRad), glm::sin(angleXRad),
							 -glm::sin(angleXRad), glm::cos(angleXRad));

	glm::mat2 rotationYMatrix(glm::cos(angleYRad), -glm::sin(angleYRad),
							  glm::sin(angleYRad), glm::cos(angleYRad));

	shader.setUniform("rotationMatrix", rotationYMatrix * rotationXMatrix);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchDark->bindTexture(GL_TEXTURE_2D, 1);
	shader.setUniform("hatchDark", 1);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchBright->bindTexture(GL_TEXTURE_2D, 2);
	shader.setUniform("hatchBright", 2);

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del plano utilizando la técnica Gooch Shading
void Plane::drawGoochShading(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", material->getKa());
	shader.setUniform("KdMaterial", material->getKd());
	shader.setUniform("KsMaterial", material->getKs());
	shader.setUniform("shininess", material->getShininess());
	shader.setUniform("Kblue", glm::vec3(0.f, 0.f, goochShading.Kblue));
	shader.setUniform("Kyellow", glm::vec3(goochShading.Kyellow, goochShading.Kyellow, 0.f));
	shader.setUniform("alpha", goochShading.alpha);
	shader.setUniform("beta", goochShading.beta);

	// - Asignar el sampleador de textura correspondiente
	shader.setUniform("TexSamplerDiffuse", 0);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	texture->bindTexture(GL_TEXTURE_2D, 0);

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del contorno del plano
void Plane::drawBasicOutline(ShaderProgram &shader, glm::mat4 mModel, 
						glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("mProjection", mProjection);
	shader.setUniform("outlineColor", basicOutline.color);
	shader.setUniform("outlineThickness", basicOutline.thickness);

	// - Dibujar plano
	vao->fillIBO(topology);
	vao->draw(GL_TRIANGLE_STRIP, topology);
}

// - Dibujado del contorno avanzado del modelo
void Plane::drawAdvancedOutline(ShaderProgram &shader, glm::mat4 mModel,
								glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visión y proyección
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("mProjection", mProjection);
	shader.setUniform("outlineColor", glm::vec3(0.f));
	shader.setUniform("outlineThickness", 0.f);
	shader.setUniform("lineExtension", 0.f);

	// - Dibujar plano
	vao->fillIBOAdjacencies(topology);
	vao->draw(GL_TRIANGLES_ADJACENCY, topology);
}