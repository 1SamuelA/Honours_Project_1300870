#ifndef _TERRAIN_SHADER_DATA_H
#define _TERRAIN_SHADER_DATA_H

#include <graphics/colour.h>
#include <vector>
#include <graphics/point_light.h>


class TerrainShaderData
{
public:
	TerrainShaderData();

	Int32 AddPointLight( const gef::PointLight& point_light );
	Int32 GetNumPointLights() const { return (Int32)point_lights_.size(); }

	inline const gef::PointLight& GetPointLight( const Int32 light_num ) const { return point_lights_[light_num]; }
	inline const gef::Colour& ambient_light_colour() const { return ambient_light_colour_; }
	inline void set_ambient_light_colour( const gef::Colour& colour ) { ambient_light_colour_ = colour; }

	void CleanUp();
private:
	gef::Colour ambient_light_colour_;
	std::vector<gef::PointLight> point_lights_;
};

#endif // _GEF_DEFAULT_3D_SHADER_H

#pragma once
