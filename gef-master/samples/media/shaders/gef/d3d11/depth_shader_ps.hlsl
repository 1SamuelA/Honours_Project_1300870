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
	float3 vertex_position : FLOAT0;
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


	float4 WHITE = float4(1.0, 1.0, 1.0, 1.0);
	float4 RED = float4(1.0, 0.0, 0.0, 1.0);
	float4 GREEN = float4(0.0, 1.0, 0.0, 1.0);
	float4 SAND = float4(76.f/256.f, 70.f/256.f, 50.f/256.f,1.0);
	float4 BLUE = float4(0.0, 0.0, 1.0, 1.0);


	float h = input.vertex_position.y;

	float4 colour = BLUE;

	if (h < 0)
	{
		colour = BLUE;

	}
	else if( (h >= 0.0f) &&(h < 5.f) )
	{
		colour = lerp( BLUE, GREEN, h / 5.0 );

	}
	else if( (h >= 5.f) && (h < 10.f) )
	{
		colour = lerp( GREEN, SAND,(h - 5.0) / 5.0 );

	}
	else if( (h >= 10.f) && (h < 15.f) )
	{
		colour = lerp( SAND, RED,(h - 10.f) / 5.f );

	}
	else if( (h >= 15.f) && (h < 20.f) )
	{
		colour = lerp( RED, WHITE, (h - 15.f) / 5.f ); ;
	}
	else if (h >= 20.f)
	{
		colour = WHITE;
	}
		

	float3 f = abs(frac(input.vertex_position.y * 1.0) - 0.5);
	float3 df = fwidth(input.vertex_position.y * 1.0);
	float mi = max(0.0, 1.0 - 1.0), ma = max(1.0, 1.0);//should be uniforms
	float3 g = clamp((f - df*mi) / (df*(ma - mi)), max(0.0, 1.0 - 1.0), 1.0);//max(0.0,1.0-gwidth) should also be sent as uniform
	float c = g.x * g.y * g.z;
	float4 gl_FragColor = float4(c, c, c, 1.0);
	gl_FragColor = gl_FragColor * colour;

	return gl_FragColor;

	//return saturate( ambient_light_colour + diffuse_colour1 + diffuse_colour2 + diffuse_colour3 + diffuse_colour4 )*diffuse_texture_colour*material_colour;
}
