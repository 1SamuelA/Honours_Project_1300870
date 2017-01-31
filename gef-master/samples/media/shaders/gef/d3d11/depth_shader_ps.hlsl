#define NUM_LIGHTS 4

cbuffer LightBuffer
{
	float4 material_colour;
	float4 ambient_light_colour;
	float4 light_colour[NUM_LIGHTS];
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;
	float2 uv : TEXCOORD0;
	float3 light_vector1 : TEXCOORD1;
	float3 light_vector2 : TEXCOORD2;
	float3 light_vector3 : TEXCOORD3;
	float3 light_vector4 : TEXCOORD4;
	float height : FLOAT0;
};

Texture2D diffuse_texture;

SamplerState Sampler0
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
float4 PS( PixelInput input ) : SV_Target
{
	float diffuse_light1 = saturate( dot( input.normal, input.light_vector1 ) );
	float diffuse_light2 = saturate( dot( input.normal, input.light_vector2 ) );
	float diffuse_light3 = saturate( dot( input.normal, input.light_vector3 ) );
	float diffuse_light4 = saturate( dot( input.normal, input.light_vector4 ) );
	float4 diffuse_texture_colour = diffuse_texture.Sample( Sampler0, input.uv );
	float4 diffuse_colour1 = diffuse_light1*light_colour[0];
	float4 diffuse_colour2 = diffuse_light2*light_colour[1];
	float4 diffuse_colour3 = diffuse_light3*light_colour[2];
	float4 diffuse_colour4 = diffuse_light4*light_colour[3];

	float h = input.height;

	float4 colour = float4(0.0, 0.0, 0.0, 1.0);

	if (h < 0)
	{
		colour = float4(0.0, 0.0, 0.0, 1.0);

	}
	else if( (h > 0) &&(h < 20.0) )
	{
		colour = clamp( h / 20.0, float4(0.0, 0.0, 0.0, 1.0), float4(1.0, 1.0, 1.0, 1.0) );

	}
	else if ( h >= 20.0 )
	{
		colour = float4(1.0, 1.0, 1.0, 1.0);
	}
		
	return colour;


	//return saturate( ambient_light_colour + diffuse_colour1 + diffuse_colour2 + diffuse_colour3 + diffuse_colour4 )*diffuse_texture_colour*material_colour;
}
