cbuffer TimeBuffer
{
	float4 time_;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 depth_position : TEXTURE0;
};

float4 PS( PixelInput input ) : SV_Target
{
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float depth_value = input.depth_position.z / input.depth_position.w;
	float4 colour;
	if (depth_value >= 0.85)
	{
		colour = float4(((depth_value*100.0f - 85.0f) / 15.0f), ((depth_value*100.0f - 85.0f) / 15.0f), ((depth_value*100.0f - 85.0f) / 15.0f), 1);
		//depth_value
	
	}
	else
	{
		colour = float4(0, 0, 0, 1);
	}
    //float4 colour = float4(depth_value, depth_value, depth_value, 1);
	// a is % of time left
	//float a = (time_.x / time_.y);
	//
	//if ((colour.r <= a)&&(colour.r >= a-0.02))
	//{
	//	colour = float4(0, 1, 0, 1);
	//}
	//else if ((colour.r < a - 0.02) && (colour.r >= a - 0.05))
	//{
	//	float b = (a*100 - ((colour.r -0.02)* 100 ))/0.03;
	//	
	//	colour = lerp(float4(0, 0, 0, 1),float4(0, 1, 0, 1),  b);
	//}
	//else
	//{
	//	colour = float4(0, 0, 0, 1);
	//}

	return colour;
}
