#include "NORMAL_TERRAIN_GEN_state.h"

#include "graphics/mesh.h"
#include <graphics\vertex_buffer.h>

#include <terrain_mesh.h>
#include <shaders\terrain_shader.h>
#include "PerlinNoise.h"
#include <random>
#include <time.h>
#include "system\platform.h"

#include "ARS_calibration_data.h"
#include "graphics\renderer_3d.h"
#include "graphics\sprite_renderer.h"
#include "graphics\font.h"

#include "input\input_manager.h"
#include "input\keyboard.h"

#include <maths\math_utils.h>

void NORMAL_TERRAIN_GENstate::init( gef::Platform * platform, ARSCalibrationData * ARSCalibration, Kinect_v2* kinect_sensor_)
{
	platform_ = platform;
	initCamera();

	initMeshes();

	


	ARSCalibration_ = ARSCalibration;
}

void NORMAL_TERRAIN_GENstate::cleanup()
{
	if( camera_0 != NULL )
	{
		delete camera_0;
		camera_0 = NULL;
	}

}

void NORMAL_TERRAIN_GENstate::Update( StateManager * state_manager, float delta_time, gef::InputManager * input_manager_ )
{

	state_manager_ = state_manager;

	delta_time_ = delta_time;
	fps_ = 1.0f / delta_time_;

	HandleInput(input_manager_);

	camera_0->SetFrameTime( delta_time_ );
	camera_0->update();

	if( terrain_changed_ )
	{
		UpdateTerrain();
	}

	gef::Matrix44 trasformation;
	gef::Matrix44 rotationx;
	gef::Matrix44 rotationy;
	gef::Matrix44 rotationz;
	gef::Matrix44 scale;
	gef::Matrix44 translation;

	trasformation.SetIdentity();
	rotationx.SetIdentity();
	rotationy.SetIdentity();
	rotationz.SetIdentity();
	scale.SetIdentity();
	translation.SetIdentity();


	rotationx.RotationX( 0.0f );
	rotationy.RotationY( 0.0f );
	rotationz.RotationZ( 0.0f );
	scale.Scale( gef::Vector4( 1.f, 1.f, 1.f ) );
	translation.SetTranslation( gef::Vector4( 0.0f, 0.0f, -0.0 ) );
	trasformation = rotationx * rotationy * rotationz *scale *translation;
	cube_player_.set_transform( trasformation );

}

void NORMAL_TERRAIN_GENstate::Render( gef::Renderer3D * renderer_3d_, gef::SpriteRenderer * sprite_renderer_, gef::Font * font )
{
	font_ = font;


	//gef::Matrix44 projection_matrix;
	//gef::Matrix44 view_matrix;

	//projection_matrix = platform_->PerspectiveProjectionFov(camera_0->GetFov(), (float)platform_->width() / (float)platform_->height(), camera_0->GetNear(), camera_0->GetFar());
	////projection_matrix = platform_.OrthographicFrustum( 0.0, platform_.width(), 0.0, platform_.height(), camera_0->GetNear(), camera_0->GetFar() );
	//view_matrix.LookAt(camera_0->GetPos(), camera_0->GetLook(), camera_0->GetUp());
	////view_matrix.LookAt(camera_eye, camera_lookat, camera_up);

	//renderer_3d_->set_projection_matrix(projection_matrix);
	//renderer_3d_->set_view_matrix(view_matrix);

	//
	//// draw meshes here
	//renderer_3d_->Begin();
	//renderer_3d_->DrawMesh(cube_player_);

	////renderer_3d_->SetFillMode(gef::Renderer3D::FillMode::kSolid);

	//renderer_3d_->End();

	RenderTerrain( renderer_3d_ );

	 //setup the sprite renderer, but don't clear the frame buffer
	 //draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD( sprite_renderer_ );
	sprite_renderer_->End();

}

void NORMAL_TERRAIN_GENstate::DrawSprite( gef::SpriteRenderer * sprite_renderer_, gef::Font * font )
{
}

void NORMAL_TERRAIN_GENstate::DrawMesh( gef::Renderer3D * renderer_3d_ )
{
}

gef::Mesh * NORMAL_TERRAIN_GENstate::CreateSquare()
{
	gef::Mesh* mesh = new gef::Mesh( *platform_ );

	// initialise the vertex data to create a 1, 1, 1 cube
	const float half_size = 0.5f;

	terrain_verticies = terrain_mesh_->GetTerrainVerticies();

	gef::Mesh::Vertex* vertices = new gef::Mesh::Vertex[terrain_verticies.size()];

	for( int i = 0; i < terrain_verticies.size(); i++ )
	{
		vertices[i] = (terrain_verticies[i]);
		vertices[i].py *= 10;
		vertices[i].py += 10;
	}

	//gef::Mesh::Vertex vertices[] = { temp_vertices[0],temp_vertices[1],temp_vertices[2],temp_vertices[3] };

	mesh->InitVertexBuffer( *platform_, vertices, terrain_verticies.size(), sizeof( gef::Mesh::Vertex ), false );

	// we will create a single triangle list primitive to draw the triangles that make up the cube
	mesh->AllocatePrimitives( 1 );
	gef::Primitive* primitive = mesh->GetPrimitive( 0 );

	terrain_index = terrain_mesh_->GetTerrainIndices();

	UInt32* indices;
	indices = new UInt32[terrain_index.size()];

	for( int i = 0; i < terrain_index.size(); i++ )
	{
		indices[i] = terrain_index[i];
	}

	primitive->InitIndexBuffer( *platform_, static_cast<const void*>(indices), terrain_index.size(), sizeof( UInt32 ) );

	//primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices)/sizeof(UInt32),sizeof(UInt32));
	primitive->set_type( gef::TRIANGLE_LIST );


	delete indices;
	indices = NULL;

	// set size of bounds, we need this for collision detection routines
	gef::Aabb aabb( gef::Vector4( -half_size, -half_size, -half_size ), gef::Vector4( half_size, half_size, half_size ) );
	gef::Sphere sphere( aabb );

	mesh->set_aabb( aabb );
	mesh->set_bounding_sphere( sphere );

	return mesh;
}

void NORMAL_TERRAIN_GENstate::initCamera()
{

	gef::Vector4 camera_eye = gef::Vector4( 0.0f, 10.0f, 0.0f );
	gef::Vector4 camera_forward = gef::Vector4( 0.0f, 9.0f, 0.0f );
	gef::Vector4 camera_up = gef::Vector4( 01.0f, 10.0f, 0.0f );
	float camera_fov = 45.0f;
	float near_plane = 0.01f;
	float far_plane = 1000.f;

	camera_0 = new CameraObject( camera_eye, camera_forward, camera_up, camera_fov, near_plane, far_plane );
	camera_0->SetYaw( 0.0f );
	camera_0->SetPitch( -90.0f );

}

void NORMAL_TERRAIN_GENstate::initMeshes()
{
	terrain_mesh_ = new TerrainMesh();

	terrain_mesh_->GenerateVertices();
	terrain_mesh_->GenerateIndex();

	terrain_shader_ = new TerrainShader( *platform_ );
	platform_->AddShader( terrain_shader_ );

	terrain_changed_ = false;

	mesh_ = CreateSquare(); 
	cube_player_.set_mesh( mesh_ );

}

void NORMAL_TERRAIN_GENstate::HandleInput( gef::InputManager* input_manager_ )
{
	const gef::Keyboard* keyboard = input_manager_->keyboard();
	if( keyboard )
	{
		// go through all the keys and print out some debug text when a key is pressed or released
		if( keyboard->IsKeyDown( gef::Keyboard::KC_ESCAPE ) )
		{
			exit( 10 );
		}


		// Translation
		if( keyboard->IsKeyDown( gef::Keyboard::KC_W ) )
		{
			camera_0->MoveForward();
		}
		if( keyboard->IsKeyDown( gef::Keyboard::KC_S ) )
		{
			camera_0->MoveBackward();
		}

		if( keyboard->IsKeyDown( gef::Keyboard::KC_A ) )
		{
			camera_0->StrafeLeft();
		}
		if( keyboard->IsKeyDown( gef::Keyboard::KC_D ) )
		{
			camera_0->StrafeRight();
		}

		// Rotation
		if( keyboard->IsKeyDown( gef::Keyboard::KC_NUMPAD4 ) )
		{
			camera_0->TurnLeft();
		}
		if( keyboard->IsKeyDown( gef::Keyboard::KC_NUMPAD6 ) )
		{
			camera_0->TurnRight();
		}

		if( keyboard->IsKeyDown( gef::Keyboard::KC_NUMPAD8 ) )
		{
			camera_0->TurnUp();
		}
		if( keyboard->IsKeyDown( gef::Keyboard::KC_NUMPAD5 ) )
		{
			camera_0->TurnDown();
		}

		if( keyboard->IsKeyDown( gef::Keyboard::KC_Q ) )
		{
			camera_0->MoveUp();
		}
		if( keyboard->IsKeyDown( gef::Keyboard::KC_E ) )
		{
			camera_0->MoveDown();
		}

		if( keyboard->IsKeyPressed( gef::Keyboard::KC_C ) )
		{

			terrain_shader_active_ = !terrain_shader_active_;
			terrain_changed_ = false;
		}


		if( keyboard->IsKeyDown( gef::Keyboard::KC_X ) )
		{
			bool Pass;
			KinectSensor_->UpdateDEFeed( Pass );

			terrain_changed_ = Pass;
		}

	}
}

void NORMAL_TERRAIN_GENstate::UpdateTerrain()
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


	mesh_->vertex_buffer()->Update( *platform_ );

	terrain_changed_ = false;
}

void NORMAL_TERRAIN_GENstate::RenderTerrain( gef::Renderer3D * renderer_3d_ )
{
	if( terrain_shader_active_ )
	{
		gef::Shader* previous_shader = renderer_3d_->shader();

		gef::Matrix44 projection_matrix;
		gef::Matrix44 view_matrix;

		float a = gef::DegToRad( 45 );
		camera_0->SetFov(45);
		//float b = camera_0->GetFov();
		// gef::DegToRad( 45 );

		//projection_matrix = platform_->PerspectiveProjectionFov( camera_0->GetFov(), (float)platform_->width() / (float)platform_->height(), camera_0->GetNear(), camera_0->GetFar() );
		
		projection_matrix = platform_->OrthographicFrustum( 50, -50,50, -50, camera_0->GetNear(), camera_0->GetFar() );

		view_matrix.LookAt( camera_0->GetPos(), camera_0->GetLook(), camera_0->GetUp() );

		// use the shader for renderering the depth values to the shadow buffer
		renderer_3d_->SetShader( terrain_shader_ );

		// render target needs to be cleared to zero [black]
		platform_->set_render_target_clear_colour( gef::Colour( 0.30f, 0.10f, 0.60f, 1.0f ) );
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
		platform_->set_render_target( NULL );

		// reset clear colour
		platform_->set_render_target_clear_colour( gef::Colour( 0.0f, 0.0f, 1.0f, 1.0f ) );

	}
}

void NORMAL_TERRAIN_GENstate::InitFont()
{
	font_ = new gef::Font( *platform_ );
	font_->Load( "comic_sans" );
}

void NORMAL_TERRAIN_GENstate::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void NORMAL_TERRAIN_GENstate::DrawHUD( gef::SpriteRenderer * sprite_renderer_ )
{
	if( font_ )
	{
		// display frame rate

		font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() - 150.f, platform_->height() - 80.f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_ );

		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 000.0f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "pitch: %.1f", camera_0->GetPitch() );
		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 030.0f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "yaw: %.1f", camera_0->GetYaw() );
		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 060.0f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "roll: %.1f", camera_0->GetRoll() );

		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 090.0f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "Pos: %.1f %.1f %.1f", camera_0->GetPos().x(), camera_0->GetPos().y(), camera_0->GetPos().z() );
	}
}