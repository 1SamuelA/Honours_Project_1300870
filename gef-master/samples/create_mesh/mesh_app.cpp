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
#include <input/keyboard.h>
#include <input\input_manager.h>
#include <maths/vector2.h>
#include <graphics/renderer_3d.h>

#include <system/debug_log.h>

#include <graphics\mesh.h>
#include <graphics\vertex_buffer.h>
#include <assets\png_loader.h>

#include <terrain_mesh.h>

#include <shaders\terrain_shader.h>

#include "StateManager.h"
#include "MAINMENU_state.h"
#include "NORMAL_TERRAIN_GEN_state.h"

#include <random>
#include <time.h>

MeshApp::MeshApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	input_manager_(NULL),
	active_touch_id_(-1),
	terrain_shader_(NULL),
	mesh_(NULL)
{
}

void MeshApp::Init()
{
	// Starts all the managers and Renders.
	sprite_renderer_ = gef::SpriteRenderer::Create( platform_ );
	renderer_3d_ = gef::Renderer3D::Create( platform_ );
	input_manager_ = gef::InputManager::Create( platform_ );
	KinectSensor_ = new Kinect_v2;

	ARS_calibration_data_ = new ARSCalibrationData;

	state_manager_ = new StateManager;

	KinectSensor_->Init();

	InitFont();

	SetupLights();

	state_manager_->AddState( new MAINMENUstate );
	state_manager_->GetCurrentState()->init( &platform_, ARS_calibration_data_, KinectSensor_ );

	previous_state = state_manager_->GetCurrentState();

	time_ = 0;
	

	
}

void MeshApp::CleanUp()
{
	if( mesh_ != NULL )
	{
		delete mesh_;
		mesh_ = NULL;
	}

	CleanUpFont();
	if( sprite_renderer_ != NULL )
	{
		delete sprite_renderer_;
		sprite_renderer_ = NULL;
	}

	if( renderer_3d_ != NULL )
	{
		delete renderer_3d_;
		renderer_3d_ = NULL;
	}

	if( terrain_shader_ != NULL )
	{
		platform_.RemoveShader( terrain_shader_ );
		delete terrain_shader_;
		terrain_shader_ = NULL;
	}
}

bool MeshApp::Update(float frame_time)
{
	input_manager_->Update();

	if( state_manager_->GetNumStates() != 0 )
	{

		if( state_manager_->GetCurrentState() == previous_state )
		{
			state_manager_->GetCurrentState()->Update( state_manager_, frame_time, input_manager_ );
		}

		if( state_manager_->GetCurrentState() != previous_state )
		{
			state_manager_->GetCurrentState()->init( &platform_, ARS_calibration_data_, KinectSensor_ );
			previous_state = state_manager_->GetCurrentState();
			state_manager_->GetCurrentState()->Update( state_manager_, frame_time, input_manager_ );
		}


	}
	return true;
}

void MeshApp::UpdateTerrain()
{
	gef::Mesh::Vertex* vertices_ = (gef::Mesh::Vertex*) mesh_->vertex_buffer()->vertex_data();

	std::vector<gef::Mesh::Vertex> temp_terrain = terrain_mesh_->GetTerrainVerticies();
	

	float increment_x, increment_y;
	increment_x = KinectSensor_->de_streams_width / terrain_mesh_->GetWidth();
	increment_y = KinectSensor_->de_streams_height / terrain_mesh_->GetHeight();

	for( int y = 0; y < terrain_mesh_->GetHeight(); y++ )
	{
		for( int x = 0; x < terrain_mesh_->GetWidth(); x++ )
		{
			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			float height = KinectSensor_->de_data_2darray[(int)increment_y * y][(int)increment_x * x] / 16.f;
			


			vertices_[(y* (int)terrain_mesh_->GetHeight()) + x].py = height;

		}
	}

	


	//vertices_[0].py = sin(time_)/2;


	mesh_->vertex_buffer()->Update( platform_ );

	terrain_changed_ = false;

}

void MeshApp::Render()
{
	
	
	state_manager_->GetCurrentState()->Render( renderer_3d_, sprite_renderer_, font_ );
	

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
	if (font_)
	{
		// display frame rate
		
		font_->RenderText(sprite_renderer_, gef::Vector4( this->platform_.width() - 150.f, this->platform_.height() - 80.f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);

		font_->RenderText(sprite_renderer_, gef::Vector4(000.0f, 000.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "pitch: %.1f", camera_0->GetPitch());
		font_->RenderText(sprite_renderer_, gef::Vector4(000.0f, 030.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "yaw: %.1f", camera_0->GetYaw());
		font_->RenderText(sprite_renderer_, gef::Vector4(000.0f, 060.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "roll: %.1f", camera_0->GetRoll());

		font_->RenderText(sprite_renderer_, gef::Vector4(000.0f, 090.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Pos: %.1f %.1f %.1f", camera_0->GetPos().x(), camera_0->GetPos().y(), camera_0->GetPos().z());
	}
}

void MeshApp::ProcessKeyboardInput()
{
	const gef::Keyboard* keyboard = input_manager_->keyboard();
	if (keyboard)
	{
		// go through all the keys and print out some debug text when a key is pressed or released
		if( keyboard->IsKeyDown( gef::Keyboard::KC_ESCAPE ) )
		{
			exit(10);
		}
		

		// Translation
		if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
		{
			camera_0->MoveForward();
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
		{
			camera_0->MoveBackward();
		}

		if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
		{
			camera_0->StrafeLeft();
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
		{
			camera_0->StrafeRight();
		}

		// Rotation
		if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD4 ))
		{
			camera_0->TurnLeft();
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD6))
		{
			camera_0->TurnRight();
		}

		if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD8))
		{
			camera_0->TurnUp();
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_NUMPAD5))
		{
			camera_0->TurnDown();
		}

		if (keyboard->IsKeyDown(gef::Keyboard::KC_Q))
		{
			camera_0->MoveUp();
		}
		if (keyboard->IsKeyDown(gef::Keyboard::KC_E))
		{
			camera_0->MoveDown();
		}

		if( keyboard->IsKeyPressed( gef::Keyboard::KC_C ) )
		{

			terrain_shader_active_ = !terrain_shader_active_;
			terrain_changed_ = false;
		}


		if (keyboard->IsKeyDown(gef::Keyboard::KC_X))
		{
			bool Pass;
			KinectSensor_->UpdateDEFeed(Pass);

			terrain_changed_ = Pass;
		}
	
	}

	
}

gef::Mesh* MeshApp::CreateCubeMesh()
{
	gef::Mesh* mesh = new gef::Mesh(platform_);

	// initialise the vertex data to create a 1, 1, 1 cube
	const float half_size = 0.5f;

	//float x_length = 4;
	//float y_length = 4;
	//
	//
	//
	//for (int y_axis = 0; y_axis < y_length; y_axis++)
	//{
	//	for (int x_axis = 0; x_axis < x_length; x_axis++)
	//	{
	//		int pos = (y_axis * x_length) + x_axis;
	//		
	//		gef::Mesh::Vertex vertex;
	//
	//		vertex.px = x_axis;
	//		vertex.py = 0.5f;// y_axis;
	//		vertex.pz = y_axis;
	//			
	//		vertex.nx = x_axis;
	//		vertex.ny = y_axis;
	//		vertex.nz = x_axis;
	//			
	//		vertex.u = 0;
	//		vertex.v = 0;
	//		
	//		terrain_verticies.push_back(vertex);
	//
	//	}
	//}

	terrain_verticies = terrain_mesh_->GetTerrainVerticies();

	gef::Mesh::Vertex* vertices = new gef::Mesh::Vertex[terrain_verticies.size()];

	for (int i = 0; i < terrain_verticies.size(); i++)
	{
		vertices[i] = (terrain_verticies[i]);
		vertices[i].py *= 10;
		vertices[i].py += 10;
	}

	//gef::Mesh::Vertex vertices[] = { temp_vertices[0],temp_vertices[1],temp_vertices[2],temp_vertices[3] };

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

	mesh->InitVertexBuffer(platform_, vertices, terrain_verticies.size(), sizeof(gef::Mesh::Vertex),false);

	// we will create a single triangle list primitive to draw the triangles that make up the cube
	mesh->AllocatePrimitives(1);
	gef::Primitive* primitive = mesh->GetPrimitive(0);

	//int indieces_size = ((x_length - 1) * (y_length - 1))* (2 * 3);
	//
	//UInt32* indices = new UInt32[indieces_size];
	//
	//int index = 0;
	//

	//for (int y_axis = 0; y_axis < y_length -1; y_axis++)
	//{
	//	for (int x_axis = 0; x_axis < x_length -1; x_axis++)
	//	{
	//
	//		/*
 	//
	//		y+1	A-----B
	//			|    /|
	//			| 1 / |
	//			|  /  |
	//			| /	  |
	//			|/	2 |
	//		y	C-----D
	//			
	//			x    x+1
	//		*/
	//		
	//		float a = ( x_length * ( y_axis + 1 ) ) + x_axis;
	//		float b = (x_length * (y_axis + 1)) + x_axis + 1;
	//		float c = (x_length * (y_axis)) + x_axis;
	//		float d = (x_length * (y_axis)) + x_axis + 1;
	//
	//
	//		//// Face 1
	//		
	//		// Upper left.
	//		terrain_index.push_back(c);
	//		
	//		// Upper right.
	//		terrain_index.push_back(b);
	//
	//		// lower left
	//		terrain_index.push_back(a);
	//
	//			
	//		//// Face 2
	//
	//		// Bottom right
	//		terrain_index.push_back(c);
	//
	//		// Upper right
	//		terrain_index.push_back(d);
	//
	//		// Lower left.
	//		terrain_index.push_back(b);
	//
	//	}
	//
	//}

	terrain_index = terrain_mesh_->GetTerrainIndices();

	UInt32* indices; 
	indices = new UInt32[terrain_index.size()];
	
	for (int i = 0; i < terrain_index.size(); i++)
	{
		indices[i] = terrain_index[i];
	}
	
	//UInt32 indicesa[] = { temp_indices[0],temp_indices[1],temp_indices[2],temp_indices[3],temp_indices[4],temp_indices[5] };
	
	//indices = indices_;
	//UInt32* index = indicesa;

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
   
	primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), terrain_index.size(), sizeof(UInt32));

	//primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices)/sizeof(UInt32),sizeof(UInt32));
	primitive->set_type(gef::TRIANGLE_LIST);


	delete indices;
	indices = NULL;

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
	default_point_light.set_colour(gef::Colour(0.1f, 0.1f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(30.0f, 30.0f, 30.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);

}

void MeshApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye = gef::Vector4(5.0f, 5.0f, 0.0f);
	camera_lookat = gef::Vector4(4.0f, 4.0f, 0.0f);
	camera_up = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov = gef::DegToRad(45.0f);
	near_plane = 0.01f;
	far_plane = 1000.f;
}

void MeshApp::RenderTerrain()
{
	if( terrain_shader_active_ )
	{
		gef::Shader* previous_shader = renderer_3d_->shader();

		gef::Matrix44 projection_matrix;
		gef::Matrix44 view_matrix;

		projection_matrix = platform_.PerspectiveProjectionFov( camera_fov, (float)platform_.width() / (float)platform_.height(), near_plane, far_plane );
		//projection_matrix = platform_.OrthographicFrustum(0 , platform_.width(),0, platform_.height(), camera_0->GetNear(), camera_0->GetFar() );

		renderer_3d_->set_projection_matrix( projection_matrix );

		view_matrix.LookAt( camera_0->GetPos(), camera_0->GetLook(), camera_0->GetUp() );
		//view_matrix.SetIdentity();
		// use the shader for renderering the depth values to the shadow buffer
		renderer_3d_->SetShader( terrain_shader_ );

		// render target needs to be cleared to zero [black]
		platform_.set_render_target_clear_colour( gef::Colour( 0.0f, 0.0f, 0.0f, 1.0f ) );
		renderer_3d_->Begin();

		float currentTime, TotalTime;
		currentTime = TotalTime = 0;
		
		


		if( renderer_3d_->shader() == terrain_shader_ )
		{
			terrain_shader_->SetSceneData( terrain_shader_->shader_data, view_matrix, projection_matrix );
			terrain_shader_->SetMeshData( cube_player_, view_matrix, projection_matrix );
			terrain_shader_->SetVertexShaderData( cube_player_.transform(), view_matrix, projection_matrix, currentTime, TotalTime );
		}
		renderer_3d_->DrawMesh( cube_player_ );

		renderer_3d_->End();

		// restore previous shader
		renderer_3d_->SetShader( previous_shader );

		// set render target to the the default [the back buffer]
		platform_.set_render_target( NULL );

		// reset clear colour
		platform_.set_render_target_clear_colour( gef::Colour( 0.0f, 0.0f, 1.0f, 1.0f ) );

	}
}

