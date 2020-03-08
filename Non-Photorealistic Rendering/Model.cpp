#include "Model.h"
#include "lodepng.h"

#include <iostream>
#include <cmath>

// - Constructor
Model::Model(std::string path)
{
	// - Cargar modelo
	loadModel(path);

	// - No es un plano
	isPlane = false;
}

// - Destructor
Model::~Model()
{
	meshes.clear();
}

// - Cargar el modelo
void Model::loadModel(std::string path)
{
	// - Leer el archivo y almacenarlo en un objeto escena. Opciones de postprocesamiento utilizadas:
	//		.aiProcess_Triangulate: transforma todas las primitivas del modelo a tri�ngulos
	//		.aiProcess_CalcTangentSpace: calcular espacio tangencial (tangentes y bitangentes)
	//      .aiProcess_GenSmoothNormals: generar normales a partir de los v�rtices si no est�n definidas
	//		.aiProcess_OptimizeMeshes: optimizar mallas
	//		.aiProcess_GenUVCoords: generar coordenadas de textura
	//		.aiProcess_JoinIdenticalVertices: reducir n�mero de v�rtices uniendo los que sean iguales

	Assimp::Importer importer;
	
	const aiScene *scene = importer.ReadFile(path, 
											 aiProcess_Triangulate |
											 aiProcess_CalcTangentSpace |
											 aiProcess_GenSmoothNormals |
											 aiProcess_OptimizeMeshes |
											 aiProcess_GenUVCoords |
											 aiProcess_JoinIdenticalVertices
	);

	// - Comprobar si han ocurrido errores
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR[ASSIMP]: " << importer.GetErrorString() << std::endl;
		return;
	}
	
	// - Almacenar la direcci�n del directorio donde se encuentra el modelo
	directory = path.substr(0, path.find_last_of('/'));

	// - Procesamiento de la escena a nivel de nodo
	processNode(scene->mRootNode, scene);
}

// - Procesamiento de la escena a nivel de nodo
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// - Procesamiento de todas las mallas que haya en el nodo
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// - Procesar malla
		aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
		Mesh *mesh = processMesh(aimesh, scene);

		// - A�adir malla
		meshes.push_back(mesh);
	}

	// - Procesamiento de los hijos del nodo (recursividad)
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

// - Procesamiento de la escena a nivel de malla
Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<PosNorm> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> topology;
	std::vector<Texture*> textures;

	bool hasNormals = false;
	bool hasTextureCoords = false;
	bool hasTangentsAndBitangents = false;

	// - Comprobar si la malla tiene normales
	if (mesh->HasNormals())
	{
		hasNormals = true;
	}

	// - Comprobar si la malla tiene coordenadas de textura
	if (mesh->mTextureCoords[0])
	{
		hasTextureCoords = true;
	}

	// - Comprobar si la malla tiene tangentes y bitangentes
	if (mesh->HasTangentsAndBitangents())
	{
		hasTangentsAndBitangents = true;
	}

	// - Procesamiento de v�rtices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		PosNorm vertex;
		glm::vec2 texCoord;

		// - Posiciones
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		// - Normales
		if (hasNormals)
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
			vertex.normal = glm::normalize(vertex.normal);
		}
		vertices.push_back(vertex);

		// - Coordenadas de textura
		if (hasTextureCoords)
		{
			// - La malla tiene coordenadas de textura
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		texCoords.push_back(texCoord);

		// - Tangentes y bitangentes
		if (hasTangentsAndBitangents)
		{
			glm::vec3 aux;

			aux.x = mesh->mTangents[i].x;
			aux.y = mesh->mTangents[i].y;
			aux.z = mesh->mTangents[i].z;

			tangents.push_back(aux);

			aux.x = mesh->mBitangents[i].x;
			aux.y = mesh->mBitangents[i].y;
			aux.z = mesh->mBitangents[i].z;

			bitangents.push_back(aux);
		}
	}

	// - Procesamiento de la topolog�a de cada malla, por cara
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace *face = &mesh->mFaces[i];

		// - Construcci�n de la topolog�a de la malla
		for (unsigned int j = 0; j < face->mNumIndices; j++)
		{
			topology.push_back(face->mIndices[j]);
		}

		// - Adyacencias: Construcci�n del mapa de ejes
		for (unsigned int j = 0; j < face->mNumIndices; j++)
		{
			// - Eje
			HalfEdge *edge = new HalfEdge();

			// - �ndices de los v�rtices que forman el eje
			std::pair<unsigned int, unsigned int> p;
			p.first = face->mIndices[j % 3];
			p.second = face->mIndices[(j + 1) % 3];

			// - V�rtice al que apunta el eje (v�rtice final del eje)
			Vertex *v = new Vertex();
			v->index = face->mIndices[(j + 1) % 3];
			v->position = vertices[v->index].position;
			edge->vertex = v;

			// - A�adir eje al mapa de ejes
			edges.insert(std::pair<std::pair<unsigned int, unsigned int>, HalfEdge*>(p, edge));
		}

		// - Adyacencias: A�adir informaci�n de adyacencia a los ejes del mapa
		for (unsigned int j = 0; j < face->mNumIndices; j++)
		{
			// - Iterador del eje actual
			auto itCurrent = edges.find(std::pair<unsigned int, unsigned int>(face->mIndices[j % 3],
																		      face->mIndices[(j + 1) % 3]));

			// - Iterador del eje siguiente al actual
			auto itNext = edges.find(std::pair<unsigned int, unsigned int>(face->mIndices[(j + 1) % 3],
																		   face->mIndices[(j + 2) % 3]));

			// - Asignar el eje siguiente como el siguiente del eje actual
			itCurrent->second->next = itNext->second;

			// - Asignar el eje siguiente como el eje que comienza con el v�rtice final del eje actual
			itCurrent->second->vertex->edge = itNext->second;

			// - Buscar el eje opuesto del actual
			auto itOpposite = edges.find(std::pair<unsigned int, unsigned int>(face->mIndices[(j + 1) % 3],
																			   face->mIndices[j % 3]));
			
			// - Asignar el eje opuesto del actual, si existe
			if (itOpposite != edges.end())
			{
				itCurrent->second->opposite = itOpposite->second;
				itOpposite->second->opposite = itCurrent->second;
			}
		}
	}

	// - Construir vector de �ndices de topolog�a con �ndices adyacentes
	//		.�ndices pares: v�rtice del tri�ngulo
	//		.�ndices impares: v�rtice no compartido del tri�ngulo vecino

	std::vector<unsigned int> adjacencyIndices;
	adjacencyIndices = buildTopologyPlusAdjacencies(topology);
	
	// - Procesamiento de materiales (texturas)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

		// - Textura del color difuso
		std::vector<Texture*> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE,
																 "textureDiffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// - Textura del color especular
		std::vector<Texture*> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR,
																  "textureSpecular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// - Textura del mapa de normales
		std::vector<Texture*> normalMaps = loadMaterialTextures(mat, aiTextureType_NORMALS,
																"textureNormal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// - Almacenar atributos del material
		loadMaterial(mat);
	}

	// - Si no tiene tangentes ni bitangentes, se devuelve la malla sin utilizarlas
	if (!hasTangentsAndBitangents)
	{
		return new Mesh(vertices, texCoords, topology, textures);
	}

	return new Mesh(vertices, texCoords, tangents, bitangents, topology, adjacencyIndices, textures);
}

// - Carga de texturas
std::vector<Texture*> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type,
												  std::string typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		bool skip = false;

		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			// - Se comparan las rutas de las texturas cargadas con las de las texturas del material actual
			//	 para evitar cargar texturas ya cargadas
			if (std::strcmp(loadedTextures[j]->getPath().data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			// - Si la textura no estaba cargada, se carga
			const char *path = str.C_Str();

			// - Creaci�n de la textura (carga y definici�n de sus par�metros)
			Texture *texture = new Texture(typeName, path);
			texture->loadImage(path, directory);
			texture->bindTexture(GL_TEXTURE_2D, i);
			texture->defineTexture(GL_TEXTURE_2D,
								   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
								   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
								   0, GL_RGBA, texture->getWidth(), texture->getHeight(),
								   0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getImage());

			texture->unbindTexture(GL_TEXTURE_2D);

			// - Se a�ade al vector de texturas procesadas
			textures.push_back(texture);

			// - Y se a�ade al vector de texturas cargadas
			loadedTextures.push_back(texture);
		}
	}
	return textures;
}

// - Carga de materiales
void Model::loadMaterial(aiMaterial* mat) 
{
	material = new Material();

	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	// - Color ambiente del material
	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material->setKa(glm::vec3(color.r, color.b, color.g));

	// - Color difuso del material
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material->setKd(glm::vec3(color.r, color.b, color.g));

	// - Color especular del material
	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material->setKs(glm::vec3(color.r, color.b, color.g));

	// - Componente shininess del material
	mat->Get(AI_MATKEY_SHININESS, shininess);
	material->setShininess(shininess);
}

// - Construir vector de �ndices con topolog�a y los �ndices de los v�rtices adyacentes
std::vector<unsigned int> Model::buildTopologyPlusAdjacencies(std::vector<unsigned int> topology)
{
	std::vector<unsigned int> indices(topology.size() * 2);

	// - Inicializar �ndices del vector con los �ndices de topolog�a
	for (unsigned int i = 0; i < topology.size(); i += 3)
	{
		indices[i * 2] = topology[i];
		indices[(i * 2) + 1] = std::numeric_limits<unsigned int>::max();
		indices[(i * 2) + 2] = topology[i + 1];
		indices[(i * 2) + 3] = std::numeric_limits<unsigned int>::max();
		indices[(i * 2) + 4] = topology[i + 2];
		indices[(i * 2) + 5] = std::numeric_limits<unsigned int>::max();
	}

	// - Asignar �ndices adyacentes
	for (unsigned int i = 0; i < indices.size(); i += 6)
	{
		// - Iterador del primer eje del tri�ngulo
		auto it = edges.find(std::pair<unsigned int, unsigned int>(indices[i], indices[i + 2]));

		// - Ejes actual, siguiente y previo
		HalfEdge *current = it->second;
		HalfEdge *next = current->next;
		HalfEdge *prev = current->next->next;

		// - A�adir el v�rtice no compartido del tri�ngulo que comparte el eje con el actual (mismo eje 
		//   en sentido opuesto) como v�rtice adyacente
		if (current->opposite != nullptr)
		{
			indices[i + 1] = current->opposite->next->vertex->index;
		}

		// - A�adir el v�rtice no compartido del tri�ngulo que comparte el eje siguiente al actual (mismo eje
		//   en sentido opuesto) como v�rtice adyacente
		if (next->opposite != nullptr)
		{
			indices[i + 3] = next->opposite->next->vertex->index;
		}

		// - A�adir el v�rtice no compartido del tri�ngulo que comparte el eje previo al actual (mismo eje
		//   en sentido opuesto) como v�rtice adyacente
		if (prev->opposite != nullptr)
		{
			indices[i + 5] = prev->opposite->next->vertex->index;
		}
	}

	// - Asignar v�rtices adyacentes a los ejes que no tienen eje opuesto (ejes frontera)
	for (unsigned int i = 0; i < indices.size(); i += 6)
	{
		if (indices[i + 1] == std::numeric_limits<unsigned int>::max())
		{
			indices[i + 1] = indices[i + 4];
		}

		if (indices[i + 3] == std::numeric_limits<unsigned int>::max())
		{
			indices[i + 3] = indices[i];
		}

		if (indices[i + 5] == std::numeric_limits<unsigned int>::max())
		{
			indices[i + 5] = indices[i + 2];
		}
	}

	return indices;
}

// - Asignar texturas de Hatching
void Model::setHatchingTextures(std::string dark, std::string bright)
{
	hatchDark = new Texture();
	hatchDark->loadImage(dark.c_str());
	hatchDark->bindTexture(GL_TEXTURE_2D, loadedTextures.size());
	hatchDark->defineTexture(GL_TEXTURE_2D,
							 GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							 GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							 0, GL_RGBA, hatchDark->getWidth(), hatchDark->getHeight(),
							 0, GL_RGBA, GL_UNSIGNED_BYTE, hatchDark->getImage());

	hatchBright = new Texture();
	hatchBright->loadImage(bright.c_str());
	hatchBright->bindTexture(GL_TEXTURE_2D, (loadedTextures.size() + 1));
	hatchBright->defineTexture(GL_TEXTURE_2D,
							   GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_REPEAT,
							   GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR,
							   0, GL_RGBA, hatchBright->getWidth(), hatchBright->getHeight(),
							   0, GL_RGBA, GL_UNSIGNED_BYTE, hatchBright->getImage());
}

// - Dibujado del modelo de forma realista
void Model::drawRealistic(ShaderProgram &shader, glm::mat4 mModel,
						  glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", material->getKa());
	shader.setUniform("KdMaterial", material->getKd());
	shader.setUniform("KsMaterial", material->getKs());
	shader.setUniform("shininess", material->getShininess());

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->drawWithTextures(shader);
	}
}

// - Dibujado del modelo de forma monocrom�tica (material)
void Model::drawMonochrome(ShaderProgram &shader, glm::mat4 mModel,
						   glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", monochrome.material.getKa());
	shader.setUniform("KdMaterial", monochrome.material.getKd());
	shader.setUniform("KsMaterial", monochrome.material.getKs());
	shader.setUniform("shininess", monochrome.material.getShininess());

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw(shader);
	}
}

// - Dibujado del modelo utilizando la t�cnica Cel-Shading
void Model::drawCelShading(ShaderProgram &shader, glm::mat4 mModel,
						   glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", material->getKa());
	shader.setUniform("KdMaterial", material->getKd());
	shader.setUniform("KsMaterial", material->getKs());
	shader.setUniform("shininess", material->getShininess());
	shader.setUniform("tones", celShading.tones);
	shader.setUniform("silhouettingFactor", celShading.silhouettingFactor);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->drawWithTextures(shader);
	}
}

// - Dibujado del modelo utilizando la t�cnica Hatching
void Model::drawHatching(ShaderProgram &shader, glm::mat4 mModel,
						 glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("KaMaterial", glm::vec3(1.0));
	shader.setUniform("KdMaterial", glm::vec3(1.0));
	shader.setUniform("KsMaterial", glm::vec3(1.0));
	shader.setUniform("shininess", 8.f);
	shader.setUniform("density", hatching.density);

	// - �ngulos de rotaci�n
	float angleXRad = glm::radians(hatching.rotationAngleX);
	float angleYRad = glm::radians(hatching.rotationAngleY);

	// - Matrices de rotaci�n
	glm::mat2 rotationXMatrix(glm::cos(angleXRad), glm::sin(angleXRad),
							 -glm::sin(angleXRad), glm::cos(angleXRad));

	glm::mat2 rotationYMatrix(glm::cos(angleYRad), -glm::sin(angleYRad),
							  glm::sin(angleYRad), glm::cos(angleYRad));

	shader.setUniform("rotationMatrix", rotationYMatrix * rotationXMatrix);

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchDark->bindTexture(GL_TEXTURE_2D, loadedTextures.size());
	shader.setUniform("hatchDark", (int) (loadedTextures.size()));

	// - Enlazar textura con la unidad de textura de GPU correspondiente
	hatchBright->bindTexture(GL_TEXTURE_2D, (loadedTextures.size() + 1));
	shader.setUniform("hatchBright", (int) (loadedTextures.size() + 1));

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw(shader);
	}
}

// - Dibujado del modelo utilizando la t�cnica Gooch Shading
void Model::drawGoochShading(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
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

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->drawWithTextures(shader);
	}
}

// - Dibujado del contorno del modelo
void Model::drawBasicOutline(ShaderProgram &shader, glm::mat4 mModel,
							 glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mvpMatrix", mProjection * mView * mModel);
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("mProjection", mProjection);
	shader.setUniform("outlineColor", basicOutline.color);
	shader.setUniform("outlineThickness", basicOutline.thickness);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->draw(shader);
	}
}

// - Dibujado del contorno avanzado del modelo
void Model::drawAdvancedOutline(ShaderProgram &shader, glm::mat4 mModel,
								glm::mat4 mView, glm::mat4 mProjection)
{
	// - Asignar matriz de modelado, visi�n y proyecci�n
	shader.setUniform("mModelView", mView * mModel);
	shader.setUniform("mProjection", mProjection);
	shader.setUniform("outlineColor", advancedOutline.color);
	shader.setUniform("outlineThickness", advancedOutline.thickness);
	shader.setUniform("lineExtension", advancedOutline.extension);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->drawAdvancedOutline(shader);
	}
}