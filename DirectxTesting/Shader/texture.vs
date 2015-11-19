// GLOBALS

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix	projectionMatrix;
};

// TYPOS
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float 2 tex : TEXCOORD0;
};

// Vertex shader
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Cambio la posicion del objeto como es una matris usa 4 cordenadas y la ultima deve ser 1 para indicar que es una traslacion
	input.position.w = 1.0f;
	
	output.position = mul(input.position , worldMatrix);
	output.position = mul(output.position , viewMatrix);
	output.position = mul(output.position , projectionMatrix);

	// Guardo las cordenadas de la textura
	output.tex = input.tex;

	return output;
}