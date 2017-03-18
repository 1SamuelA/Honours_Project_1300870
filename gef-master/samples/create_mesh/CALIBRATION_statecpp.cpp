#include "CALIBRATION_state.h"

#include "graphics/mesh.h"
#include <graphics\vertex_buffer.h>

#include <terrain_mesh.h>
#include <shaders\terrain_shader.h>
#include <shaders\UI_layer_shader.h>
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
#include "hand_collision.h"

#include "ConfigFile.h"

#include <iostream>



#define CALIBRATIONSTAGES 6

void CALIBRATIONstate::init( gef::Platform * platform, ARSCalibrationData * ARSCalibration, Kinect_v2* kinect_sensor_ )
{
	ARSCalibration_ = ARSCalibration;

	platform_ = platform;
	initCamera();

	initMeshes();

	KinectSensor_ = kinect_sensor_;
	updateKinect = false;

	Hand_Collision = false;



	if( !AlreadyInit )
	{

		ConfigFile cfg( "config.cfg" );

		ARSCalibration_->MinDepth = cfg.getValueOfKey<double>( "Min_Sand_Depth" );
		ARSCalibration_->maxDepth = cfg.getValueOfKey<double>( "Max_Sand_Depth" );

		ARSCalibration_->ForgroundMinDepth = cfg.getValueOfKey<double>( "ForgroundMinDepth" );
		ARSCalibration_->ForgroundMaxDepth = cfg.getValueOfKey<double>( "ForgroundMaxDepth" );

		double x, y, z, w;

		x = cfg.getValueOfKey<double>( "Left" );
		y = cfg.getValueOfKey<double>( "Right" );
		z = cfg.getValueOfKey<double>( "Top" );
		w = cfg.getValueOfKey<double>( "Bottom" );

		ARSCalibration_->LeftRightTopBottom = gef::Vector4( x, y, z, w );

		x = cfg.getValueOfKey<double>( "Image_Left" );
		y = cfg.getValueOfKey<double>( "Image_Right" );
		z = cfg.getValueOfKey<double>( "Image_Top" );
		w = cfg.getValueOfKey<double>( "Image_Bottom" );

		ARSCalibration_->Image_LeftRightTopBottom = gef::Vector4( x, y, z, w );

		ARSCalibration_->rotation.set_x(cfg.getValueOfKey<double>( "rotation" ));

		camera_0->TurnRight( ARSCalibration_->rotation.x() );
		//ARSCalibration_->MinDepth = 2000;
		//ARSCalibration_->maxDepth = 2200;
		//
		//ARSCalibration_->LeftRightTopBottom = gef::Vector4( -50, 50, 50, -50 );
		//
		//ARSCalibration_->Image_LeftRightTopBottom = gef::Vector4( 0, KinectSensor_->cDepthWidth, KinectSensor_->cDepthHeight, 0 );

		AlreadyInit = !AlreadyInit;

	}

	FirstBox = new HandCollision(gef::Vector4(0,0,50,50));

	
	

	calibration_mode = 0;
}

void CALIBRATIONstate::cleanup()
{
	if( camera_0 != NULL )
	{
		delete camera_0;
		camera_0 = NULL;
	}

}

void CALIBRATIONstate::Update( StateManager * state_manager, float delta_time, gef::InputManager * input_manager_ )
{



	state_manager_ = state_manager;

	delta_time_ = delta_time;
	fps_ = 1.0f / delta_time_;

	HandleInput( input_manager_ );



	if( updateKinect )
	{
		if( timer_depth_update <= 0 )
		{
			bool Pass;
			KinectSensor_->UpdateDEFeed( Pass , ARSCalibration_->MinDepth , ARSCalibration_->maxDepth );
			terrain_changed_ = Pass;

			timer_depth_update = 0.1f;

		}
		else
		{
			timer_depth_update -= delta_time;
			if( terrain_changed_Calibration == true )
			{
				terrain_changed_ = true;
			}
		}
	}

	camera_0->SetFrameTime( delta_time_ );
	camera_0->update();



	switch( calibration_mode )
	{
	case 0 :
	{
		


		break;
	}
	case 1:
	{
		break;
	}

	}



	if( terrain_changed_ )
	{
		//UpdateTerrain();
		UpdateTerrain();
		UpdateDepthLayer( forground_terrain, forground_mesh_, ARSCalibration_->ForgroundMinDepth, ARSCalibration_->ForgroundMaxDepth );
	}

	HandCollisionUpdate( forground_terrain, forground_mesh_, ARSCalibration_->ForgroundMinDepth, ARSCalibration_->ForgroundMaxDepth );

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
	translation.SetTranslation( gef::Vector4( 0.0f, -49.0f, -0.0 ) );
	trasformation = rotationx * rotationy * rotationz *scale *translation;
	forground_meshinstance_.set_transform( trasformation );

}

void CALIBRATIONstate::Render( gef::Renderer3D * renderer_3d_, gef::SpriteRenderer * sprite_renderer_, gef::Font * font )
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
	sprite_renderer_->Begin( false );
	DrawHUD( sprite_renderer_ );
	sprite_renderer_->End();

}

void CALIBRATIONstate::DrawSprite( gef::SpriteRenderer * sprite_renderer_, gef::Font * font )
{
}

void CALIBRATIONstate::DrawMesh( gef::Renderer3D * renderer_3d_ )
{
}

gef::Mesh * CALIBRATIONstate::CreateSquare()
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

void CALIBRATIONstate::initCamera()
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

void CALIBRATIONstate::initMeshes()
{
	terrain_mesh_ = new TerrainMesh();

	terrain_mesh_->GenerateVertices();
	terrain_mesh_->GenerateIndex();

	terrain_shader_ = new TerrainShader( *platform_ );
	platform_->AddShader( terrain_shader_ );

	terrain_changed_ = false;

	mesh_ = CreateSquare();
	cube_player_.set_mesh( mesh_ );

	//  Forground
	forground_terrain = new TerrainMesh();

	forground_terrain->GenerateVertices();
	forground_terrain->GenerateIndex();

	UI_shader = new UILayerShader( *platform_ );
	platform_->AddShader( UI_shader );

	terrain_changed_ = false;

	forground_mesh_ = CreateSquare();
	forground_meshinstance_.set_mesh( forground_mesh_ );

}

void CALIBRATIONstate::HandleInput( gef::InputManager* input_manager_ )
{
	const gef::Keyboard* keyboard = input_manager_->keyboard();
	if( keyboard )
	{
		// go through all the keys and print out some debug text when a key is pressed or released
		if( keyboard->IsKeyDown( gef::Keyboard::KC_P ) )
		{
			state_manager_->GotoPreviousState();
		}

		switch( calibration_mode )
		{
		case 0:
		{
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_NUMPAD4 ) )
			{
				camera_0->TurnLeft( 90.f );
			}
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_NUMPAD6 ) )
			{
				camera_0->TurnRight( 90.f );
			}
			break;
		}
		case 1:
		{
			gef::Vector4 a = ARSCalibration_->LeftRightTopBottom;

			if( keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_x( a.x() + 1 );
			}
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_x( a.x() - 1 );
			}

			if( keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_z( a.z() - 1 );
			}
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_z( a.z() + 1 );
			}
			break;
		}
		case 2:
		{
			gef::Vector4 a = ARSCalibration_->LeftRightTopBottom;

			if( keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_y( a.y() + 1 );
			}
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_y( a.y() - 1 );
			}

			if( keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_w( a.w() - 1 );
			}
			if( keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->LeftRightTopBottom.set_w( a.w() + 1 );
			}
			break;
		}
		case 3:
		{
			gef::Vector4 a = ARSCalibration_->Image_LeftRightTopBottom;

			if( keyboard->IsKeyDown( gef::Keyboard::KC_W ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.x() < ARSCalibration_->Image_LeftRightTopBottom.y() )
					ARSCalibration_->Image_LeftRightTopBottom.set_x( a.x() + 1 );
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_S ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.x() > 0 )
					ARSCalibration_->Image_LeftRightTopBottom.set_x( a.x() - 1 );
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_A ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.y() > ARSCalibration_->Image_LeftRightTopBottom.x() )
					ARSCalibration_->Image_LeftRightTopBottom.set_y( a.y() - 1 );
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_D ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.y() <  KinectSensor_->cDepthWidth )
					ARSCalibration_->Image_LeftRightTopBottom.set_y( a.y() + 1 );
			}
			break;
		}
		case 4:
		{
			gef::Vector4 a = ARSCalibration_->Image_LeftRightTopBottom;

			if( keyboard->IsKeyDown( gef::Keyboard::KC_W ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.z() < KinectSensor_->cDepthHeight )
					ARSCalibration_->Image_LeftRightTopBottom.set_z( a.z() + 1 );
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_S ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.z() > ARSCalibration_->Image_LeftRightTopBottom.w() + 1 )
					ARSCalibration_->Image_LeftRightTopBottom.set_z( a.z() - 1 );
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_A ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.w() > 0 )
					ARSCalibration_->Image_LeftRightTopBottom.set_w( a.w() - 1 );
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_D ) )
			{
				if( ARSCalibration_->Image_LeftRightTopBottom.w() < ARSCalibration_->Image_LeftRightTopBottom.z() - 1 )
					ARSCalibration_->Image_LeftRightTopBottom.set_w( a.w() + 1 );
			}
			break;
		}
		case 5:
		{
			gef::Vector4 a = ARSCalibration_->LeftRightTopBottom;

			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->MinDepth += 1;
			}
			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->MinDepth -= 1;
			}

			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->maxDepth -= 1;
			}
			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->maxDepth += 1;
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->MinDepth += 10;
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->MinDepth -= 10;
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->maxDepth -= 10;
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->maxDepth += 10;
			}
			break;
		}
		case 6:
		{
			gef::Vector4 a = ARSCalibration_->LeftRightTopBottom;

			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->ForgroundMinDepth += 1;
			}
			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->ForgroundMinDepth -= 1;
			}

			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->ForgroundMaxDepth -= 1;
			}
			if( !keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->ForgroundMaxDepth += 1;
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
			{
				ARSCalibration_->ForgroundMinDepth += 10;
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
			{
				ARSCalibration_->ForgroundMinDepth -= 10;
			}

			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_A ) )
			{
				ARSCalibration_->ForgroundMaxDepth -= 10;
			}
			if( keyboard->IsKeyDown( gef::Keyboard::KC_LSHIFT ) && keyboard->IsKeyPressed( gef::Keyboard::KC_D ) )
			{
				ARSCalibration_->ForgroundMaxDepth += 10;
			}
			break;
		}
		}
		
		

		


		if( keyboard->IsKeyPressed( gef::Keyboard::KC_X ) )
		{

			updateKinect = !updateKinect;


		}
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_Z ) )
		{

			terrain_changed_Calibration = !terrain_changed_Calibration;


		}
		
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_C ) )
		{
			if( calibration_mode > 0 )
				calibration_mode--;
		}

		if( keyboard->IsKeyPressed( gef::Keyboard::KC_V ) )
		{
			if( calibration_mode < CALIBRATIONSTAGES )
				calibration_mode++;

		}


		






	}
}

void CALIBRATIONstate::HandCollisionUpdate( TerrainMesh* DepthLayerMesh, gef::Mesh* depthLayerMesh, float minDepth, float maxDepth )
{

	Hand_Collision = false;

	gef::Mesh::Vertex* vertices_ = (gef::Mesh::Vertex*) depthLayerMesh->vertex_buffer()->vertex_data();
	std::vector<gef::Mesh::Vertex> temp_terrain = DepthLayerMesh->GetTerrainVerticies();


	float depth = 50;


	float increment_x, increment_y;
	increment_x = (ARSCalibration_->Image_LeftRightTopBottom.y() - ARSCalibration_->Image_LeftRightTopBottom.x())
		/ DepthLayerMesh->GetWidth();
	increment_y = (ARSCalibration_->Image_LeftRightTopBottom.z() - ARSCalibration_->Image_LeftRightTopBottom.w())
		/ DepthLayerMesh->GetHeight();

	for( int y = 0; y < DepthLayerMesh->GetHeight(); y++ )
	{
		for( int x = 0; x < DepthLayerMesh->GetWidth(); x++ )
		{

			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			
			float depth = vertices_[(y* (int)terrain_mesh_->GetHeight()) + x].py;
			if( depth > 20 )
			{
				if( FirstBox->Collision( gef::Vector2( x - 50, y - 50 ) ))
				{
					Hand_Collision = true;
					break;
				}
				else
				{
					false;
				}
			}

		}
	}

	//vertices_[0].py = sin(time_)/2;


	depthLayerMesh->vertex_buffer()->Update( *platform_ );

	terrain_changed_ = false;


}

void CALIBRATIONstate::HandleCameraUpdates(const gef::Keyboard* keyboard )
{

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
}



void CALIBRATIONstate::UpdateTerrain()
{
	float Range = (float)ARSCalibration_->maxDepth - (float)ARSCalibration_->MinDepth;

	gef::Mesh::Vertex* vertices_ = (gef::Mesh::Vertex*) mesh_->vertex_buffer()->vertex_data();
	std::vector<gef::Mesh::Vertex> temp_terrain = terrain_mesh_->GetTerrainVerticies();

	float increment_x, increment_y;
	increment_x = (ARSCalibration_->Image_LeftRightTopBottom.y() - ARSCalibration_->Image_LeftRightTopBottom.x()) 
		/ terrain_mesh_->GetWidth();
	increment_y = (ARSCalibration_->Image_LeftRightTopBottom.z() - ARSCalibration_->Image_LeftRightTopBottom.w())
		/ terrain_mesh_->GetHeight();

	for( int y = 0; y < terrain_mesh_->GetHeight(); y++ )
	{
		for( int x = 0; x < terrain_mesh_->GetWidth(); x++ )
		{

			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			int a = (ARSCalibration_->Image_LeftRightTopBottom.w() + (increment_y * y));
			int b = (ARSCalibration_->Image_LeftRightTopBottom.x() + (increment_x * x));


			float depth = KinectSensor_->de_data_2darray[a][b];

			float DepthValueInRange = depth - ARSCalibration_->MinDepth;

			DepthValueInRange = (Range - DepthValueInRange);
			float  depthval = (DepthValueInRange / Range) * 25;

			if ((depth > ARSCalibration_->MinDepth))
				vertices_[(y* (int)terrain_mesh_->GetHeight()) + x].py = depthval;

		}
	}

	//vertices_[0].py = sin(time_)/2;


	mesh_->vertex_buffer()->Update( *platform_ );

	terrain_changed_ = false;
}

void CALIBRATIONstate::UpdateDepthLayer( TerrainMesh* DepthLayerMesh, gef::Mesh* depthLayerMesh, float minDepth, float maxDepth )
{
	float Range = maxDepth - minDepth;

	gef::Mesh::Vertex* vertices_ = (gef::Mesh::Vertex*) depthLayerMesh->vertex_buffer()->vertex_data();
	std::vector<gef::Mesh::Vertex> temp_terrain = DepthLayerMesh->GetTerrainVerticies();

	float increment_x, increment_y;
	increment_x = (ARSCalibration_->Image_LeftRightTopBottom.y() - ARSCalibration_->Image_LeftRightTopBottom.x())
		/ DepthLayerMesh->GetWidth();
	increment_y = (ARSCalibration_->Image_LeftRightTopBottom.z() - ARSCalibration_->Image_LeftRightTopBottom.w())
		/ DepthLayerMesh->GetHeight();

	for( int y = 0; y < DepthLayerMesh->GetHeight(); y++ )
	{
		for( int x = 0; x < DepthLayerMesh->GetWidth(); x++ )
		{

			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			int a = (ARSCalibration_->Image_LeftRightTopBottom.w() + (increment_y * y));
			int b = (ARSCalibration_->Image_LeftRightTopBottom.x() + (increment_x * x));


			float depth = KinectSensor_->de_data_2darray[a][b];

			float DepthValueInRange = depth - minDepth;

			DepthValueInRange = (Range - DepthValueInRange);
			float  depthval = (DepthValueInRange / Range)*25;


			if( (depth > minDepth) )
				vertices_[(y* (int)terrain_mesh_->GetHeight()) + x].py = depthval;
			else
				vertices_[(y* (int)terrain_mesh_->GetHeight()) + x].py = 0;

		}
	}

	//vertices_[0].py = sin(time_)/2;


	depthLayerMesh->vertex_buffer()->Update( *platform_ );

	terrain_changed_ = false;
}



void CALIBRATIONstate::RenderTerrain( gef::Renderer3D * renderer_3d_ )
{
	if( terrain_shader_active_ )
	{
		gef::Shader* previous_shader = renderer_3d_->shader();

		gef::Matrix44 projection_matrix;
		gef::Matrix44 view_matrix;

		float a = gef::DegToRad( 45 );
		camera_0->SetFov( 45 );
		//float b = camera_0->GetFov();
		// gef::DegToRad( 45 );

		//projection_matrix = platform_->PerspectiveProjectionFov( camera_0->GetFov(), (float)platform_->width() / (float)platform_->height(), camera_0->GetNear(), camera_0->GetFar() );

		projection_matrix = platform_->OrthographicFrustum( 
			ARSCalibration_->LeftRightTopBottom.x(),
			ARSCalibration_->LeftRightTopBottom.y(),
			ARSCalibration_->LeftRightTopBottom.z(),
			ARSCalibration_->LeftRightTopBottom.w(),
			camera_0->GetNear(), camera_0->GetFar());

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
			terrain_shader_->SetVertexShaderData( cube_player_.transform(), view_matrix, projection_matrix, 0, TotalTime );
		}
		
		renderer_3d_->DrawMesh( cube_player_ );

		if( renderer_3d_->shader() == terrain_shader_ )
		{
			terrain_shader_->SetSceneData( terrain_shader_->shader_data, view_matrix, projection_matrix );
			terrain_shader_->SetMeshData( forground_meshinstance_, view_matrix, projection_matrix );
			terrain_shader_->SetVertexShaderData( forground_meshinstance_.transform(), view_matrix, projection_matrix, 1, TotalTime );
		}
		
		renderer_3d_->DrawMesh( forground_meshinstance_ );


		renderer_3d_->End();

		// restore previous shader
		renderer_3d_->SetShader( previous_shader );

		// set render target to the the default [the back buffer]
		platform_->set_render_target( NULL );

		// reset clear colour
		platform_->set_render_target_clear_colour( gef::Colour( 0.0f, 0.0f, 1.0f, 1.0f ) );

	}
}

void CALIBRATIONstate::InitFont()
{
	font_ = new gef::Font( *platform_ );
	font_->Load( "comic_sans" );
}

void CALIBRATIONstate::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void CALIBRATIONstate::DrawHUD( gef::SpriteRenderer * sprite_renderer_ )
{
	if( font_ )
	{
		// display frame rate

		switch( calibration_mode )
		{
		case 0:
		{
			// Camera Rotation
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width()/2, platform_->height()/2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "Camera Rotation");
			break;
		}
		case 1:
		{
			// OrthoView topLeft
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "OrthoView topLeft" );
			break;
		}
		case 2:
		{
			// OrthoView bottomRight
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "OrthoView bottomRight" );
			break;
		}
		case 3:
		{
			//Image Width
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "Image Width" );
			break;
		}
		case 4:
		{
			//Image Height
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "Image Height" );
			break;
		}
		case 5:
		{
			// Sand Depth
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "Sand Depth" );

			font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 120.0f, -1.1f ), 1.0f, 0xffff00ff, gef::TJ_LEFT, "Min, Max Depth: %i %i", ARSCalibration_->MinDepth, ARSCalibration_->maxDepth );
			break;
		}
		case 6:
		{
			// Forground Depth
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, platform_->height() / 2, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT
				, "Forground Depth" );

			font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 120.0f, -1.1f ), 1.0f, 0xffff00ff, gef::TJ_LEFT, "Min, Max Depth: %i %i", ARSCalibration_->ForgroundMinDepth, ARSCalibration_->ForgroundMaxDepth );
			break;
		}

		}

		if( Hand_Collision )
		{
			font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() / 2, 20+platform_->height() / 2,  -1.1f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "Collsion" );

		}

		font_->RenderText( sprite_renderer_, gef::Vector4( platform_->width() - 150.f, platform_->height() - 80.f, -1.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_ );

		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 000.0f, -1.1f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "pitch: %.1f", camera_0->GetPitch() );
		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 030.0f, -1.1f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "yaw: %.1f", camera_0->GetYaw() );
		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 060.0f, -1.1f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "roll: %.1f", camera_0->GetRoll() );
																		   
		font_->RenderText( sprite_renderer_, gef::Vector4( 000.0f, 090.0f, -1.1f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "Pos: %.1f %.1f %.1f", camera_0->GetPos().x(), camera_0->GetPos().y(), camera_0->GetPos().z() );
	

	}
}

bool CALIBRATIONstate::OutputConfigFile()
{
	//std::ofstream file;
	//file.open( fName.c_str() );
	//if( !file )
	//	exit( -1 );
	std::ofstream file;

	class OutData
	{
	private:
		std::string varible_name_, varible_data_, _comment;
	public:
		OutData( std::string name, std::string data, std::string coment = NULL )
		{
			varible_name_ = name;
			varible_data_ = data;
		}

	};

	
	std::vector<OutData> OutputData;

	OutputData.push_back( OutData( "Min_Sand_Depth", std::to_string( ARSCalibration_->MinDepth ) ) );
	OutputData.push_back( OutData( "Max_Sand_Depth", std::to_string( ARSCalibration_->maxDepth ) ) );
	OutputData.push_back( OutData( "", NULL ) );
	OutputData.push_back( OutData( "Min_Sand_Depth", std::to_string( ARSCalibration_->MinDepth ) ) );
	OutputData.push_back( OutData( "Min_Sand_Depth", std::to_string( ARSCalibration_->MinDepth ) ) );


	for( int x = 0; x < OutputData.size(); x++ )
	{
		file << "s";
	}
	return true;
}