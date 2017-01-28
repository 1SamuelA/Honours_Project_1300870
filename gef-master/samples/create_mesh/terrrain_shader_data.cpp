#include "terrrain_shader_data.h"

TerrainShaderData::TerrainShaderData():
ambient_light_colour_( 0.0f, 0.0f, 0.0f, 1.f )
{

}

Int32 TerrainShaderData::AddPointLight( const gef::PointLight& point_light )
{
	point_lights_.push_back( point_light );
	return (Int32)point_lights_.size() - 1;
}

void TerrainShaderData::CleanUp()
{
	point_lights_.clear();
}