cbuffer TimeBuffer
{
	float4 time_;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 depth_position : TEXTURE0;
	float4 colour : COLOR;
};

float4 PS( PixelInput input ) : SV_Target
{
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float depth_value = input.depth_position.z / input.depth_position.w;
	depth_value /= 2;

	return depth_value;
}
