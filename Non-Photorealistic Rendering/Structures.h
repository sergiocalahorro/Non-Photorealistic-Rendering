#pragma once
#include <glm.hpp>
#include "Material.h"

// - Estructura VBO entrelazado
struct PosNorm
{
	glm::vec3 position;
	glm::vec3 normal;
};


// - Estructura de datos Half-Edge

// - Forward declarations
typedef struct HalfEdge HalfEdge;
typedef struct Vertex Vertex;

// - Estructura de datos Half-Edge: Eje
struct HalfEdge
{
	Vertex* vertex;			// - Vértice al que apunta el eje (vértice final)
	HalfEdge* opposite;		// - Eje opuesto 
	HalfEdge* next;			// - Siguiente eje alrededor de la cara (en sentido antihorario)
};

// - Estructura de datos Half-Edge: Vértice
struct Vertex
{
	unsigned int index;		// - Índice del vértice
	glm::vec3 position;		// - Coordenadas del vértice
	HalfEdge *edge;			// - Eje que comienza con este vértice
};

// - Contorno básico
struct BasicOutline
{
	glm::vec3 color;
	float thickness;
	bool enabled;

	BasicOutline()
	{
		this->color = glm::vec3(0.f);
		this->thickness = 0.f;
		this->enabled = false;
	}

	BasicOutline(glm::vec3 color, float thickness)
	{
		this->color = color;
		this->thickness = thickness;
		this->enabled = false;
	}
};

// - Contorno avanzado
struct AdvancedOutline
{
	glm::vec3 color;
	float thickness;
	float extension;
	bool enabled;

	AdvancedOutline()
	{
		this->color = glm::vec3(0.f);
		this->thickness = 0.f;
		this->extension = 0.f;
		this->enabled = false;
	}

	AdvancedOutline(glm::vec3 color, float thickness, float extension)
	{
		this->color = color;
		this->thickness = thickness;
		this->extension = extension;
		this->enabled = false;
	}
};

// - Técnica monocromo
struct MonochromeTechnique
{
	Material material;

	MonochromeTechnique()
	{
		this->material = Material();
	}

	MonochromeTechnique(Material material)
	{
		this->material = material;
	}
};

// - Técnica Cel-Shading
struct CelShadingTechnique
{
	float tones;
	float silhouettingFactor;

	CelShadingTechnique()
	{
		this->tones = 2.f;
		this->silhouettingFactor = 0.f;
	}

	CelShadingTechnique(float tones)
	{
		this->tones = tones;
		this->silhouettingFactor = 0.f;
	}

	CelShadingTechnique(float tones, float silhouettingFactor)
	{
		this->tones = tones;
		this->silhouettingFactor = silhouettingFactor;
	}
};

// - Técnica Halftone
struct HalftoneTechnique
{
	float threshold;
	float intensity;
	float size;
	glm::vec3 color;

	HalftoneTechnique()
	{
		this->threshold = 0.25f;
		this->intensity = 0.7f;
		this->size = 2.f;
		this->color = glm::vec3(0.1f);
	}
};

// - Técnica Dithering
struct DitheringTechnique
{
	float threshold1;
	float threshold2;
	float threshold3;
	float threshold4;

	float intensityThreshold1;
	float intensityThreshold2;
	float intensityThreshold3;
	float intensityThreshold4;

	float density;
	float width;

	bool useSceneColor;

	DitheringTechnique()
	{
		this->threshold1 = 0.8f;
		this->threshold2 = 0.6f;
		this->threshold3 = 0.3f;
		this->threshold4 = 0.15f;

		this->intensityThreshold1 = 0.9f;
		this->intensityThreshold2 = 0.6f;
		this->intensityThreshold3 = 0.3f;
		this->intensityThreshold4 = 0.f;

		this->density = 5.f;
		this->width = 2.f;

		this->useSceneColor = false;
	}
};

// - Técnica Hatching
struct HatchingTechnique
{
	float density;
	float rotationAngleX;
	float rotationAngleY;
	float rotationAngleZ;

	HatchingTechnique()
	{
		this->density = 6.f;
		this->rotationAngleX = 0.f;
		this->rotationAngleY = 0.f;
		this->rotationAngleZ = 0.f;
	}

	HatchingTechnique(float density, float rotationAngleX, float rotationAngleY)
	{
		this->density = density;
		this->rotationAngleX = rotationAngleX;
		this->rotationAngleY = rotationAngleY;
		this->rotationAngleZ = 0.f;
	}

	HatchingTechnique(float density, float rotationAngleX, float rotationAngleY, float rotationAngleZ)
	{
		this->density = density;
		this->rotationAngleX = rotationAngleX;
		this->rotationAngleY = rotationAngleY;
		this->rotationAngleZ = rotationAngleZ;
	}
};

// - Técnica Charcoal
struct CharcoalTechnique
{
	bool sobelFilter;
	float threshold;
	glm::vec3 edgeColor;
	float colorMultiplier;
	float noise;

	CharcoalTechnique()
	{
		this->sobelFilter = true;
		this->threshold = 0.025f;
		this->edgeColor = glm::vec3(0.2f);
		this->colorMultiplier = 3.f;
		this->noise = 0.3f;
	}
};

// - Técnica GoochShading
struct GoochShadingTechnique
{
	float Kblue;
	float Kyellow;
	
	float alpha;
	float beta;

	GoochShadingTechnique()
	{
		this->Kblue = 0.6f;
		this->Kyellow = 0.9f;
		this->alpha = 0.1f;
		this->beta = 0.5f;
	}

	GoochShadingTechnique(float Kblue, float Kyellow, float alpha, float beta)
	{
		this->Kblue = Kblue;
		this->Kyellow = Kyellow;
		this->alpha = alpha;
		this->beta = beta;
	}
};

// - Técnica Painterly
struct PainterlyTechnique
{
	int brushSize;

	PainterlyTechnique()
	{
		this->brushSize = 4;
	}
};

// - Técnica Pixel Art
struct PixelArtTechnique
{
	float numHorizontalPixels;
	float numVerticalPixels;

	PixelArtTechnique()
	{
		this->numHorizontalPixels = 200.f;
		this->numVerticalPixels = 200.f;
	}
};