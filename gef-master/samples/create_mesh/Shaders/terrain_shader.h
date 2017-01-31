#ifndef _TERRAIN_SHADER_H
#define _TERRAIN_SHADER_H

#include <graphics/shader.h>
#include <gef.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include "terrrain_shader_data.h"

#define MAX_NUM_POINT_LIGHTS 4

namespace gef
{
	class MeshInstance;
	class Matrix44;
	class Primitive;
	class Texture;
	class Material;
	class Platform;
}


class TerrainShader : public gef::Shader
{
public:
	struct VertexShaderData
	{
		gef::Matrix44 wvp;
	};

	struct MeshData
	{
		gef::Matrix44 wvp;
		gef::Matrix44 world;
		gef::Matrix44 invworld;
		gef::Vector4 ambient_light_colour;
		gef::Vector4 light_position[MAX_NUM_POINT_LIGHTS];
		gef::Vector4 light_colour[MAX_NUM_POINT_LIGHTS];
	};

	struct PrimitiveData
	{
		gef::Vector4 material_colour;
		const gef::Texture* material_texture;
	};

	TerrainShaderData shader_data;

	TerrainShader(const gef::Platform& platform);
	virtual ~TerrainShader();

	void SetVertexShaderData(const gef::Matrix44& world_matrix, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix, float time_, float total_time);
	void SetSceneData(TerrainShaderData shaderdata, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix );
	void SetMeshData(const gef::MeshInstance& mesh_instance, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix);
	void SetMaterialData( const gef::Material* material );

	inline PrimitiveData& primitive_data() { return primitive_data_; }
protected:

	Int32 wvp_matrix_variable_index_;
	Int32 world_matrix_variable_index_;
	Int32 invworld_matrix_variable_index_;
	Int32 light_position_variable_index_;

	Int32 material_colour_variable_index_;
	Int32 ambient_light_colour_variable_index_;
	Int32 light_colour_variable_index_;

	Int32 time_float_variable_index_;

	Int32 texture_sampler_index_;

	MeshData mesh_data_;
	PrimitiveData primitive_data_;

	gef::Matrix44 view_projection_matrix_;
};

#endif // _DEPTH_SHADER_H