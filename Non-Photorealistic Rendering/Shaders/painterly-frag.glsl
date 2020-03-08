#version 400

in vec2 texCoord;

// - Textura de la escena
uniform sampler2D TexScene;

// - Painterly
uniform int brushSize;

vec3 colorArray[4];
vec3 colorSquareArray[4];

layout (location = 0) out vec4 FragColor;

void main()
{
	vec2 texSize = textureSize(TexScene, 0);

	float brushSizeSquare = float((brushSize + 1) * (brushSize + 1));

    for (int i = -brushSize; i <= 0; i++)
    {
		for (int j = -brushSize; j <= 0; j++)
		{
            vec3 texColor = texture2D(TexScene, texCoord + vec2(i, j) / texSize).rgb;
            colorArray[0] += texColor;
            colorSquareArray[0] += texColor * texColor;
        }
    }

	for (int i = -brushSize; i <= 0; i++)
    {
		for (int j = 0; j <= brushSize; j++)
		{
            vec3 texColor = texture2D(TexScene, texCoord + vec2(i, j) / texSize).rgb;
            colorArray[1] += texColor;
            colorSquareArray[1] += texColor * texColor;
        }
    }

    for (int i = 0; i <= brushSize; i++)
    {
		for (int j = -brushSize; j <= 0; j++)
		{
            vec3 texColor = texture2D(TexScene, texCoord + vec2(i, j) / texSize).rgb;
            colorArray[2] += texColor;
            colorSquareArray[2] += texColor * texColor;
        }
    }

    for (int i = 0; i <= brushSize; i++)
    {
		for (int j = 0; j <= brushSize; j++)
		{
            vec3 texColor = texture2D(TexScene, texCoord + vec2(i, j) / texSize).rgb;
            colorArray[3] += texColor;
            colorSquareArray[3] += texColor * texColor;
        }
    }

	float minValue = 1.f;

    for (int i = 0; i < 4; i++)
    {
        colorArray[i] /= brushSizeSquare;
        colorSquareArray[i] = abs((colorSquareArray[i] / brushSizeSquare) - colorArray[i] * colorArray[i]);
		
        float sumColorSquare = colorSquareArray[i].r + colorSquareArray[i].g + colorSquareArray[i].b;

        if (sumColorSquare < minValue)
        {
            minValue = sumColorSquare;
            FragColor = vec4(colorArray[i], 1.0);
        }
    }
}