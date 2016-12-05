// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(cb0)
{
	float4 diffuseColour;
	float4 ambientColour;
	float3 lightDirection;
	float padding;
};


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour;

	colour = input.colour;
	colour = colour*ambientColour;


	// Set the default output color to the ambient light value for all pixels.

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		colour += (diffuseColour * lightIntensity);
	}

	// Saturate the final light color.
	colour = saturate(colour);


	float4 grassColor;
	float4 slopeColor;
	float4 rockColor;
	float blendAmount;
	float4 textureColor;
	float slope;
	
	//// Sample the grass color from the texture using the sampler at this texture coordinate location.
	//grassColor = grassTexture.Sample(SampleType, input.tex);
	//
	//// Sample the slope color from the texture using the sampler at this texture coordinate location.
	//slopeColor = slopeTexture.Sample(SampleType, input.tex);
	//
	//// Sample the rock color from the texture using the sampler at this texture coordinate location.
	//rockColor = rockTexture.Sample(SampleType, input.tex);


	


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	// textureColor = texture0.Sample(Sampler0, input.tex);
	
	//colour = float4(input.normal.x, input.normal.y, input.normal.z,1);
	return colour;
}