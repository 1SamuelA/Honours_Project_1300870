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
		gef::Matrix44 wvp = world_matrix * view_matrix * projection_matrix;
		gef::Matrix44 wvpT;

		wvpT.Transpose(wvp);

		gef::Vector4 time_;

		time_.set_x(time);
		time_.set_y(total_time);
		time_.set_z(5);
		time_.set_w(1);
		
		device_interface_->SetVertexShaderVariable(wvp_matrix_variable_index_, &wvpT);
		device_interface_->SetPixelShaderVariable(time_float_variable_index_, &time_);
	}
