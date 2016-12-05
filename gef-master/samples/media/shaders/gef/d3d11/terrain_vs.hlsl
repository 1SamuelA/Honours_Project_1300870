// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer
{
	matrix wvp;
	matrix world;
	matrix invworld;
};

struct VertexInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour: COLOR;
};

OutputType main(InputType input)
{
	OutputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, wvp);
	

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(invworld,input.normal );

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	float4 colour;

	if (input.position.y < -0.7)
	{
		colour = float4(0.1, 1, 1, 1);
	}
	else if (input.position.y < -0.3)
	{
		float blendAmount = input.position.y*0.1;
		colour = lerp(float4(1, 0, 1, 1), float4(0.1, 1, 1, 1), blendAmount);
	}
	else if (input.position.y < 0.0)
	{
		float blendAmount = (input.position.y*0.1);
		colour = lerp(float4(1, 1, 0, 1), float4(1, 0, 1, 1), blendAmount);
	}
	else if (input.position.y < 0.3)
	{
		float blendAmount = 0.1;
		colour = float4(1, 1, 0, 1);
	}
	else if (input.position.y < 0.7)
	{
		float blendAmount = 0.1;
		colour = lerp(float4(1, 1, 1, 1), float4(0, 1, 1, 1), blendAmount);
	}
	else
	{
		colour = float4(1, 1, 1, 1);
	}
	
	output.colour = colour;
	return output;
}