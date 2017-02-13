#include "MAINMENU_state.h"

#include "NORMAL_TERRAIN_GEN_state.h"

#include "graphics\renderer_3d.h"
#include "graphics\sprite_renderer.h"
#include "graphics\font.h"
#include "system\platform.h"

#include "graphics\primitive.h"
#include <graphics/scene.h>
#include <graphics/sprite.h>
#include <graphics\image_data.h>

#include "system\debug_log.h"
#include "graphics\render_target.h"
#include <maths\math_utils.h>
#include "graphics\material.h"
#include "graphics\texture.h"
#include <input\input_manager.h>
#include <input\keyboard.h>

#include "graphics\mesh.h"
#include <maths\math_utils.h>

#define DEBUGON true

void MAINMENUstate::init(gef::Platform* platform, ARSCalibrationData* ARSCalibration, 
	Kinect_v2* kinect_sensor_ )
{
	platform_ = platform;

	ARS_calibration_data_ = ARSCalibration;

	gef::PNGLoader PngLoader;
	//gef::ImageData crossData;
	//crossHairs = new gef::Sprite();
	//PngLoader.Load("Calibration_Square.png", *platform_, crossData);
	//if (crossData.image() == NULL)
	//{
	//	exit(-1);
	//}
	//crossHairsTexture = gef::Texture::Create(*platform_, crossData);
	//crossHairs->set_height(64);
	//crossHairs->set_width(64);
	//crossHairs->set_position(gef::Vector4( platform_->width() / 2, platform_->height() - 28, 0.9f));
	//crossHairs->set_texture(crossHairsTexture);
	//crossHairs->set_uv_width(1.0f);
	//crossHairs->set_uv_height(1.0f);

	
	// Init Meshes ///////////////////////////////////
	if( !AlreadyInit )
	{
		InitMeshes();

		gef::ImageData textureData;
		PngLoader.Load( "menu/selectiontexture.png", *platform_, textureData );
		if( textureData.image() == NULL )
		{
			exit( -1 );
		}
		selectionTexture = gef::Texture::Create( *platform_, textureData );
		defselectmat = new gef::Material();
		//defselectmat->set_texture(selectionTexture);
		defselectmat->set_colour( 0xffffffff );

		selectedmat = new gef::Material();
		//selectedmat->set_texture(selectionTexture);
		selectedmat->set_colour( 0xff00ff00 );

		unavaliblemat = new gef::Material();
		unavaliblemat->set_colour( 0xff0000ff );
	}

	

	


	// Selection animations.
	SelectionCounter = 0.0f;
	SelectionMax = 3;
	Selection = 0;

	
}

void MAINMENUstate::InitMeshes()
{
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

	//  Main Menu
	model_scene_title = new gef::Scene();
	model_scene_title->ReadSceneFromFile(*platform_, "menu/mainmenu.scn");
	model_scene_title->CreateMaterials(*platform_);
	if (model_scene_title->meshes.size() > 0)
		mesh_title = model_scene_title->CreateMesh(*platform_, model_scene_title->meshes.front());
	//mesh_title = CreateSquare();
	draw_list.push_back(new GameObject());
	draw_list.back()->set_mesh(mesh_title);


	rotationx.RotationX(0.0f);
	rotationy.RotationY(0.0f);
	rotationz.RotationZ(0.0f);
	scale.Scale(gef::Vector4(0.1f, 0.1f, 0.1f));
	translation.SetTranslation(gef::Vector4(0.0f, 0.25f, -1.0));
	trasformation = rotationx * rotationy * rotationz *scale *translation;
	draw_list.back()->set_transform(trasformation);


	trasformation.SetIdentity();
	rotationx.SetIdentity();
	rotationy.SetIdentity();
	rotationz.SetIdentity();
	scale.SetIdentity();
	translation.SetIdentity();

	//  Play
	model_scene_play = new gef::Scene();
	model_scene_play->ReadSceneFromFile(*platform_, "menu/play.scn");
	model_scene_play->CreateMaterials(*platform_);
	if (model_scene_play->meshes.size() > 0)
		mesh_play = model_scene_play->CreateMesh(*platform_, model_scene_play->meshes.front());

	//mesh_title = CreateSquare();
	draw_list.push_back(new GameObject());
	draw_list.back()->set_mesh(mesh_play);

	rotationx.RotationX(FRAMEWORK_PI / 2);
	rotationy.RotationY(0.0f);
	rotationz.RotationZ(0.0f);
	scale.Scale(gef::Vector4(0.05f, 0.05f, 0.052f));
	translation.SetTranslation(gef::Vector4(0.0f, 0.125f, -1.0));
	trasformation = rotationx * rotationy * rotationz *scale *translation;
	draw_list.back()->set_transform(trasformation);

	trasformation.SetIdentity();
	rotationx.SetIdentity();
	rotationy.SetIdentity();
	rotationz.SetIdentity();
	scale.SetIdentity();
	translation.SetIdentity();

	//  options
	model_scene_options = new gef::Scene();
	model_scene_options->ReadSceneFromFile(*platform_, "menu/options.scn");
	model_scene_options->CreateMaterials(*platform_);
	if (model_scene_options->meshes.size() > 0)
		mesh_options = model_scene_play->CreateMesh(*platform_, model_scene_options->meshes.front());
	draw_list.push_back(new GameObject());
	draw_list.back()->set_mesh(mesh_options);

	rotationx.RotationX(FRAMEWORK_PI / 2);
	rotationy.RotationY(0.0f);
	rotationz.RotationZ(0.0f);
	scale.Scale(gef::Vector4(0.05f, 0.05f, 0.052f));
	translation.SetTranslation(gef::Vector4(0.0f, 0.0f, -1.0));
	trasformation = rotationx * rotationy * rotationz *scale * translation;
	draw_list.back()->set_transform(trasformation);

	trasformation.SetIdentity();
	rotationx.SetIdentity();
	rotationy.SetIdentity();
	rotationz.SetIdentity();
	scale.SetIdentity();
	translation.SetIdentity();

	// Exit
	model_scene_exit = new gef::Scene();
	model_scene_exit->ReadSceneFromFile(*platform_, "menu/exit.scn");
	model_scene_exit->CreateMaterials(*platform_);
	if (model_scene_exit->meshes.size() > 0)
		mesh_exit = model_scene_play->CreateMesh(*platform_, model_scene_exit->meshes.front());
	draw_list.push_back(new GameObject());
	draw_list.back()->set_mesh(mesh_exit);

	rotationx.RotationX(FRAMEWORK_PI);
	rotationy.RotationY(0.0f);
	rotationz.RotationZ(0.0f);
	scale.Scale(gef::Vector4(0.05f, 0.05f, 0.052f));
	translation.SetTranslation(gef::Vector4(0.0f, -0.125f, -1.0));
	trasformation = rotationx * rotationy * rotationz *scale * translation;
	draw_list.back()->set_transform(trasformation);

}

void MAINMENUstate::Render3DScene( gef::Renderer3D * renderer_3d_ )
{

}

void MAINMENUstate::cleanup()
{
	delete platform_;
	platform_ = NULL;
}

void MAINMENUstate::Update(StateManager* state_manager, float delta_time, gef::InputManager* input_manager_)
{

	state_manager_ = state_manager;

	delta_time_ = delta_time;
	fps_ = 1.0f / delta_time_;

	HandleInput(input_manager_);

	// Updates all the Marker Infomation.

	
	//Update Material
	mesh_title->GetPrimitive(0)->set_material(defselectmat);
	mesh_play->GetPrimitive(0)->set_material(defselectmat); 
	mesh_options->GetPrimitive(0)->set_material(defselectmat);
	mesh_exit->GetPrimitive(0)->set_material(defselectmat);

	switch(Selection)
	{
	case 0:
	{
		
		mesh_play->GetPrimitive(0)->set_material(selectedmat);
		break;
	}
	case 1:
	{
		mesh_options->GetPrimitive(0)->set_material(selectedmat);
		break;
	}
	case 2:
	{
		mesh_exit->GetPrimitive(0)->set_material(selectedmat);
		break;
	}
	}

}

void MAINMENUstate::HandleInput(gef::InputManager* input_manager_)
{
	const gef::Keyboard* keyboard = input_manager_->keyboard();
	if( keyboard )
	{
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_ESCAPE ) )
		{
			exit( 0 );
		}
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_S ) )
		{
			if( Selection >= SelectionMax - 1 )
				Selection = 0;
			else
				Selection++;
		}
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_W ) )
		{
			if( Selection <= 0 )
				Selection = 2;
			else
				Selection--;
		}
		if( keyboard->IsKeyPressed( gef::Keyboard::KC_RETURN ) )
		{
			switch( Selection )
			{
			case 0:
			{
				state_manager_->AddState(new NORMAL_TERRAIN_GENstate());
				break;
			}
			case 1:
			{
				break;
			}
			case 2:
			{
				exit( -1 );
				break;
			}
			}
		}


	}

	/*
	if ( input_manager_ ->keyboard())
	{
		if (controller->buttons_pressed() & (gef_SONY_CTRL_START))
		{
			exit(0);
		}
		if (controller->buttons_pressed() & (gef_SONY_CTRL_DOWN))
		{
			if (Selection >= SelectionMax-1)
				Selection = 0;
			else
				Selection++;
		}
		if (controller->buttons_pressed() & (gef_SONY_CTRL_UP))
		{
			if (Selection <= 0)
				Selection = 2;
			else
				Selection--;
		}
		if (controller->buttons_pressed() & (gef_SONY_CTRL_CROSS))
		{
			switch (Selection)
			{
			case 0:
			{
				break;
			}
			case 1:
			{
				break;
			}
			case 2:
			{
				exit(-1);
				break;
			}
			}
		}


	}
	*/
}

void MAINMENUstate::Render(gef::Renderer3D * renderer_3d_, gef::SpriteRenderer * sprite_renderer_, gef::Font * font)
{	

	DrawSprite(sprite_renderer_, font);

	DrawMesh(renderer_3d_);

}

void MAINMENUstate::DrawSprite(gef::SpriteRenderer * sprite_renderer_, gef::Font * font)
{
	font_ = font;
	gef::Matrix44 proj_matrix2d;
	
	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);

	sprite_renderer_->Begin(false);
	


	if (font_)
	{

		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		
		font_->RenderText(sprite_renderer_, gef::Vector4(800.0f, 20.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "MAIN MENU");

		//if (any_marker_visible_)
		//{
		//	font_->RenderText(sprite_renderer_, gef::Vector4(400.0f, 20.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_CENTRE, "MARKER FOUND");
		//}

		

	}
	sprite_renderer_->End();

}

void MAINMENUstate::DrawMesh(gef::Renderer3D * renderer_3d)
{
	//renderer_3d->set_projection_matrix(camera_feed_->Get_final_marker_proj_());
	//renderer_3d->set_view_matrix(camera_feed_->Get_identity_matrix_());
	renderer_3d->Begin(false);
	
	for (int MeshCount = 0; MeshCount < draw_list.size(); MeshCount++)
	{
		renderer_3d->DrawMesh(*draw_list[MeshCount]);
	}
	
	renderer_3d->End();
}

gef::Mesh* MAINMENUstate::CreateSquare()
{
	gef::Mesh* mesh = new gef::Mesh( *platform_ );

	// initialise the vertex data to create a 1, 1, 1 cube
	const float half_size = 0.059 / 2.0f;
	const gef::Mesh::Vertex vertices[] = {
		{ half_size, -half_size, 0.0f,  0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
		{ half_size,  half_size, 0.0f,  0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
		{ -half_size,  half_size,0.0f, -0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
		{ -half_size, -half_size,0.0f, -0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
		{ half_size, -half_size,  half_size*2.0,  0.577f, -0.577f,  0.577f, 0.0f, 0.0f },
		{ half_size,  half_size,  half_size*2.0,  0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
		{ -half_size,  half_size,  half_size*2.0, -0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
		{ -half_size, -half_size,  half_size*2.0, -0.577f, -0.577f,  0.577f, 0.0f, 0.0f }
	};

	mesh->InitVertexBuffer(*platform_, static_cast<const void*>(vertices), sizeof(vertices) / sizeof(gef::Mesh::Vertex), sizeof(gef::Mesh::Vertex));

	// we will create a single triangle list primitive to draw the triangles that make up the cube
	mesh->AllocatePrimitives(1);
	gef::Primitive* primitive = mesh->GetPrimitive(0);

	const UInt32 indices[] = {
		// Back
		0, 1, 2,
		2, 3, 0,
		// Front
		6, 5, 4,
		4, 7, 6,
		// Left
		2, 7, 3,
		2, 6, 7,
		// Right
		0, 4, 1,
		5, 1, 4,
		// Top
		6, 2, 1,
		5, 6, 1,
		// Bottom
		0, 3, 7,
		0, 7, 4
	};

	primitive->InitIndexBuffer(*platform_, static_cast<const void*>(indices), sizeof(indices) / sizeof(UInt32), sizeof(UInt32));
	primitive->set_type(gef::TRIANGLE_LIST);

	// set size of bounds, we need this for collision detection routines
	gef::Aabb aabb(gef::Vector4(-half_size, -half_size, -half_size), gef::Vector4(half_size, half_size, half_size));
	gef::Sphere sphere(aabb);

	mesh->set_aabb(aabb);
	mesh->set_bounding_sphere(sphere);

	return mesh;
}

void MAINMENUstate::InitFont()
{
	font_ = new gef::Font( *platform_ );
	font_->Load( "comic_sans" );
}

void MAINMENUstate::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void MAINMENUstate::DrawHUD( gef::SpriteRenderer * sprite_renderer_ )
{
	if( font_ )
	{
		// display frame rate

		font_->RenderText( sprite_renderer_, gef::Vector4(platform_->width() - 150.f, platform_->height() - 80.f, -0.9f ), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_ );

	}
}

