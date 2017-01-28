#include "terrain_shader.h"
#include <graphics/shader_interface.h>
#include <graphics/mesh_instance.h>
#include <graphics/primitive.h>
#include <system/debug_log.h>
#include <graphics/mesh.h>

TerrainShader::TerrainShader(const gef::Platform& platform)
		:gef::Shader(platform)
		, wvp_matrix_variable_index_(-1)
		, time_float_variable_index_(-1)
		, invworld_matrix_variable_index_(-1)
		, world_matrix_variable_index_(-1)
	{
		bool success = true;

		// load vertex shader source in from a file
		char* vs_shader_source = NULL;
		Int32 vs_shader_source_length = 0;
		success = LoadShader("depth_shader_vs", "shaders/gef", &vs_shader_source, vs_shader_source_length, platform);

		char* ps_shader_source = NULL;
		Int32 ps_shader_source_length = 0;
		success = LoadShader("depth_shader_ps", "shaders/gef", &ps_shader_source, ps_shader_source_length, platform);
		

		device_interface_->SetVertexShaderSource(vs_shader_source, vs_shader_source_length);
		device_interface_->SetPixelShaderSource(ps_shader_source, ps_shader_source_length);

		delete[] vs_shader_source;
		vs_shader_source = NULL;
		delete[] ps_shader_source;
		ps_shader_source = NULL;



		wvp_matrix_variable_index_ = device_interface_->AddVertexShaderVariable("wvp", gef::ShaderInterface::kMatrix44);
		world_matrix_variable_index_ = device_interface_->AddVertexShaderVariable("world", gef::ShaderInterface::kMatrix44);
		invworld_matrix_variable_index_ = device_interface_->AddVertexShaderVariable("invworld", gef::ShaderInterface::kMatrix44);

		// pixel shader variables
		time_float_variable_index_ = device_interface_->AddPixelShaderVariable("time", gef::ShaderInterface::kVector4);

		device_interface_->AddVertexParameter("position", gef::ShaderInterface::kVector3, 0, "POSITION", 0);

		// Uncomment for x64
		device_interface_->AddVertexParameter("normal", gef::ShaderInterface::kVector3, 12, "NORMAL", 0);
		device_interface_->AddVertexParameter("uv", gef::ShaderInterface::kVector2, 24, "TEXCOORD", 0);

		
		device_interface_->set_vertex_size(sizeof(gef::Mesh::Vertex));
		
		device_interface_->CreateVertexFormat();

		success = device_interface_->CreateProgram();
	}

TerrainShader::~TerrainShader()
	{
	}

	void TerrainShader::SetVertexShaderData(const gef::Matrix44& world_matrix, const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix, float time, float total_time)
	{
		
		gef::Vector4 time_;

		time_.set_x(time);
		time_.set_y(total_time);
		time_.set_z(5);
		time_.set_w(1);
		
		device_interface_->SetPixelShaderVariable(time_float_variable_index_, &time_);
	}

	void TerrainShader::SetSceneData( TerrainShaderData shaderdata)
	{
		////gef::Matrix44 wvp = world_matrix * view_matrix * projection_matrix;
		//gef::Vector4 light_positions[MAX_NUM_POINT_LIGHTS];
		//gef::Vector4 light_colours[MAX_NUM_POINT_LIGHTS];
		//gef::Vector4 ambient_light_colour = shader_data.ambient_light_colour().GetRGBAasVector4();

		//for( Int32 light_num = 0; light_num < MAX_NUM_POINT_LIGHTS; ++light_num )
		//{
		//	Vector4 light_position;
		//	Colour light_colour;
		//	if( light_num < shader_data.GetNumPointLights() )
		//	{
		//		const PointLight& point_light = shader_data.GetPointLight( light_num );
		//		light_position = point_light.position();
		//		light_colour = point_light.colour();
		//	}
		//	else
		//	{
		//		// no light data
		//		// set this light to a light with no colour
		//		light_position = Vector4( 0.0f, 0.0f, 0.0f );
		//		light_colour = Colour( 0.0f, 0.0f, 0.0f );
		//	}
		//	light_positions[light_num] = Vector4( light_position.x(), light_position.y(), light_position.z(), 1.f );
		//	light_colours[light_num] = light_colour.GetRGBAasVector4();
		//}

		//view_projection_matrix_ = view_matrix * projection_matrix;


		//device_interface_->SetVertexShaderVariable( light_position_variable_index_, (float*)light_positions );
		//device_interface_->SetPixelShaderVariable( ambient_light_colour_variable_index_, (float*)&ambient_light_colour );
		//device_interface_->SetPixelShaderVariable( light_colour_variable_index_, (float*)light_colours );

	}

	void TerrainShader::SetMeshData(const gef::MeshInstance& mesh_instance ,const gef::Matrix44& view_matrix, const gef::Matrix44& projection_matrix)
	{
		gef::Matrix44 view_projection_matrix_ = view_matrix*projection_matrix;

		// calculate world view projection matrix
		gef::Matrix44 wvp = mesh_instance.transform() * view_projection_matrix_;

		// calculate the transpose of inverse world matrix to transform normals in shader
		gef::Matrix44 inv_world;
		inv_world.Inverse(mesh_instance.transform());
		//inv_world_transpose_matrix.Transpose(inv_world);

		// take transpose of matrices for the shaders
		gef::Matrix44 wvpT, worldT;

		wvpT.Transpose(wvp);
		worldT.Transpose(mesh_instance.transform());
		// taking the transpose of the inverse world transpose matrix, just give use the inverse world matrix
		// no need to waste calculating that here

		device_interface_->SetVertexShaderVariable(wvp_matrix_variable_index_, &wvpT);
		device_interface_->SetVertexShaderVariable(world_matrix_variable_index_, &worldT);
		device_interface_->SetVertexShaderVariable(invworld_matrix_variable_index_, &inv_world);
	}

