#include "mesh_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>

#include <graphics\mesh.h>
#include <graphics\vertex_buffer.h>

MeshApp::MeshApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL)
{
}

void MeshApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	InitFont();

	mesh_ = CreateCubeMesh();
	cube_player_.set_mesh(mesh_);

	SetupCamera();
	SetupLights();

	time_ = 0;
}

void MeshApp::CleanUp()
{
	delete mesh_;
	mesh_ = NULL;

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

bool MeshApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	time_ += frame_time;

	gef::Mesh::Vertex* vertices_ = (gef::Mesh::Vertex*) mesh_->vertex_buffer()->vertex_data();

	//vertices_[0].py = sin(time_);
	
	mesh_->vertex_buffer()->Update(platform_);


	return true;
}

void MeshApp::Render()
{
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov, (float)platform_.width() / (float)platform_.height(), near_plane, far_plane);
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw meshes here
	renderer_3d_->Begin();
	renderer_3d_->DrawMesh(cube_player_);
	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}
void MeshApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void MeshApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void MeshApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

gef::Mesh* MeshApp::CreateCubeMesh()
{
	gef::Mesh* mesh = new gef::Mesh(platform_);

	// initialise the vertex data to create a 1, 1, 1 cube
	const float half_size = 0.5f;

	float x_length = 2;
	float y_length = 2;

	

	for (int y_axis = 0; y_axis < y_length; y_axis++)
	{
		for (int x_axis = 0; x_axis < x_length; x_axis++)
		{
			int pos = (y_axis * x_length) + x_axis;
			
			gef::Mesh::Vertex vertex;

			vertex.px = x_axis;
			vertex.py = 0.5f;// y_axis;
			vertex.pz = y_axis;
				
			vertex.nx = x_axis;
			vertex.ny = y_axis;
			vertex.nz = x_axis;
				
			vertex.u = 0;
			vertex.v = 0;
			
			terrain_verticies.push_back(vertex);

		}
	}


	gef::Mesh::Vertex* temp_vertices = new gef::Mesh::Vertex[terrain_verticies.size()];

	for (int i = 0; i < terrain_verticies.size(); i++)
	{
		temp_vertices[i] = terrain_verticies[i];
	}

	const gef::Mesh::Vertex vertices[] = { temp_vertices[0],temp_vertices[1],temp_vertices[2],temp_vertices[3] };

	//{
	//	{ half_size, -half_size, -half_size,  0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
	//	{ half_size,  half_size, -half_size,  0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
	//	{ -half_size,  half_size, -half_size, -0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
	//	{ -half_size, -half_size, -half_size, -0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
	//	{ half_size, -half_size,  half_size,  0.577f, -0.577f,  0.577f, 0.0f, 0.0f },
	//	{ half_size,  half_size,  half_size,  0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
	//	{ -half_size,  half_size,  half_size, -0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
	//	{ -half_size, -half_size,  half_size, -0.577f, -0.577f,  0.577f, 0.0f, 0.0f }
	//};//new gef::Mesh::Vertex[8];
	
	 

	mesh->InitVertexBuffer(platform_, static_cast<const void*>(vertices), sizeof(vertices)/sizeof(gef::Mesh::Vertex), sizeof(gef::Mesh::Vertex),false);

	// we will create a single triangle list primitive to draw the triangles that make up the cube
	mesh->AllocatePrimitives(1);
	gef::Primitive* primitive = mesh->GetPrimitive(0);

	//int indieces_size = ((x_length - 1) * (y_length - 1))* (2 * 3);
	//
	//UInt32* indices = new UInt32[indieces_size];
	//
	//int index = 0;
	//

	for (int y_axis = 0; y_axis < y_length -1; y_axis++)
	{
		for (int x_axis = 0; x_axis < x_length -1; x_axis++)
		{
	
			/*
 	
			y+1	A-----B
				|    /|
				| 1 / |
				|  /  |
				| /	  |
				|/	2 |
			y	C-----D
				
				x    x+1
			*/
			
			float a = ( x_length * ( y_axis + 1 ) ) + x_axis;
			float b = (x_length * (y_axis + 1)) + x_axis + 1;
			float c = (x_length * (y_axis)) + x_axis;
			float d = (x_length * (y_axis)) + x_axis + 1;
	
	
			//// Face 1
			
			// Upper left.
			terrain_index.push_back(c);
			
			// Upper right.
			terrain_index.push_back(b);
	
			// lower left
			terrain_index.push_back(a);
	
				
			//// Face 2
	
			// Bottom right
			terrain_index.push_back(c);
	
			// Upper right
			terrain_index.push_back(d);
	
			// Lower left.
			terrain_index.push_back(b);
	
		}
	
	}


	UInt32* temp_indices = new UInt32[terrain_index.size()];

	for (int i = 0; i < terrain_index.size(); i++)
	{
		temp_indices[i] = terrain_index[i];
	}



	//UInt32* indices; 
	//indices = new UInt32[6];
	
	//for (int i = 0; i < terrain_index.size(); i++)
	//{
	//	indices[0] = terrain_index[i];
	//}
	
	UInt32 indices[] = { temp_indices[0],temp_indices[1],temp_indices[2],temp_indices[3],temp_indices[4],temp_indices[5] };
	
	
	//indices = indices_;
	UInt32* index = indices;

	//const UInt32 indices[] = {
	//	// Back
	//	0, 1, 2,
	//	2, 3, 0,
	//	// Front
	//	6, 5, 4,
	//	4, 7, 6,
	//	// Left
	//	2, 7, 3,
	//	2, 6, 7,
	//	// Right
	//	0, 4, 1,
	//	5, 1, 4,
	//	// Top
	//	6, 2, 1,
	//	5, 6, 1,
	//	// Bottom
	//	0, 3, 7,
	//	0, 7, 4
	//};
   

	primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices)/sizeof(UInt32),sizeof(UInt32));
	primitive->set_type(gef::TRIANGLE_LIST);


//	delete [] indices;
//	indices = NULL;

	// set size of bounds, we need this for collision detection routines
	gef::Aabb aabb(gef::Vector4(-half_size, -half_size, -half_size), gef::Vector4(half_size, half_size, half_size));
	gef::Sphere sphere(aabb);

	mesh->set_aabb(aabb);
	mesh->set_bounding_sphere(sphere);

	return mesh;
}

void MeshApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void MeshApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye = gef::Vector4(5.0f, 5.0f, 5.0f);
	camera_lookat = gef::Vector4(0.0f, 0.0f, 0.0f);
	camera_up = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov = gef::DegToRad(45.0f);
	near_plane = 0.01f;
	far_plane = 1000.f;
}
