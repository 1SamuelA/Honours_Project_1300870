#ifndef _TERRAIN_SHADER_H
#define _TERRAIN_SHADER_H

#include <graphics/shader.h>
#include <maths/matrix44.h>
#include "terrrain_shader_data.h"

#define MAX_NUM_POINT_LIGHTS 4

namespace gef
{
	class MeshInstance;
	class Primitive;
	class Platform;
	
}


class TerrainShader : public gef::Shader
{
public:
	struct VertexShaderData
	{
		gef::Matrix44 wvp;
	};

	TerrainShaderData shaderdata;

	TerrainShader(const gef::Platform& platform);
	virtual ~TerrainShader();

	void SetVertexShaderData(const gef::Matrix44& world_matrix, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix, float time_, float total_time);
	void SetSceneData(TerrainShaderData shaderdata);
	void SetMeshData(const gef::MeshInstance& mesh_instance, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix);
protected:

	Int32 wvp_matrix_variable_index_;
	Int32 world_matrix_variable_index_;
	Int32 invworld_matrix_variable_index_;
	Int32 time_float_variable_index_;


	Int32 lightPosition[4];
	Int32 lightDirection[4];
	Int32 lightDefuseColour[4];
	Int32 lightAmbientColour[4];


};

#endif // _DEPTH_SHADER_H