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

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 depth_position : TEXTURE0;
	float4 colour : COLOR;
};

void VS( in VertexInput input,
         out PixelInput output )
{
    output.position = mul(input.position, wvp);
    output.depth_position = output.position;

	float4 colour;

	if (input.position.y < -1.0)
	{
		colour = float4(0.1, 1, 1, 1);
	}
	else if (input.position.y < -0.5)
	{
		float blendAmount = input.position.y*0.1;
		colour = lerp(float4(0, 1, 1, 1), float4(0.1, 1, 1, 1), blendAmount);
	}
	else if (input.position.y < 0.0)
	{
		
		colour = float4(1, 1, 0, 1);
	}
	
	else if (input.position.y < 0.3)
	{
		float blendAmount = (input.position.y*0.1);
		colour = lerp(float4(0, 00.5,0, 1), float4(1, 1, 0, 1), blendAmount);
	}
	else if (input.position.y < 1.3)
	{
		float blendAmount = 0.1;
		colour = float4(00, 0.5, 0, 1);
	}
	else if (input.position.y < 1.7)
	{
		float blendAmount = 0.1;
		colour = lerp(float4(1, 1, 1, 1), float4(0, 0.5, 0, 1), blendAmount);
	}
	else
	{
		colour = float4(1, 1, 1, 1);
	}
	output.colour = colour;

}
