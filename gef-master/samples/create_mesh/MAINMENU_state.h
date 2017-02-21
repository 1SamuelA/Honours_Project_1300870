#pragma once
#include "State.h"
#include <vector>
#include "game_object.h"

#include "ARS_calibration_data.h"
#include <graphics/scene.h>
#include "assets\png_loader.h"
#include "Shaders\terrain_shader.h"
#include "StateManager.h"

#include "mouse_collision_object.h"

namespace gef
{
	class Mesh;
	class Aabb;
	class Platform;
	class Renderer3D;
	class SpriteRender;
	class Font;
	class Sprite;
	class PNGLoader;
	class Texture;
	class InputManager;
	class SonyController;
}

class MAINMENUstate : public State
{
public:
	MAINMENUstate()
	{
		AlreadyInit = false;
	};

	~MAINMENUstate() {};

	void init( gef::Platform* platform, ARSCalibrationData* ARS_calibration_data, Kinect_v2* kinect_sensor_ );
	void cleanup();
	void Update(StateManager* state_manager_,float delta_time, gef::InputManager* inputmanager);
	void Render(class gef::Renderer3D* renderer_3d_, class gef::SpriteRenderer* sprite_renderer_, gef::Font* font);
	void DrawSprite(class gef::SpriteRenderer* sprite_renderer_, gef::Font* font);
	void DrawMesh(class gef::Renderer3D* renderer_3d_);
	gef::Mesh* CreateSquare();
private:
	
	void InitFont();
	void CleanUpFont();
	void DrawHUD( gef::SpriteRenderer * sprite_renderer_ );

	bool AlreadyInit;

	void HandleInput(gef::InputManager* input_manager_);
	void HandleMouseInput(gef::InputManager* input_manager_);

	ARSCalibrationData* ARS_calibration_data_;

	float fps_;

	int RenderPoint;
	bool RenderMarkerFound;
	
	StateManager* state_manager_;

	bool mouse_collision_(gef::Vector2 top_left_, gef::Vector2 bottom_right_);

	gef::PNGLoader* PngLoader;

	gef::Sprite* crossHairs;
	gef::Texture* crossHairsTexture;


	gef::Platform* platform_;
	gef::Font* font_;

	std::vector<GameObject*> draw_list;

	
	void InitMeshes();
	

	gef::Scene* model_scene_title;
	gef::Mesh* mesh_title;

	gef::Scene* model_scene_play;
	gef::Mesh* mesh_play;

	gef::Scene* model_scene_options;
	gef::Mesh* mesh_options;

	gef::Scene* model_scene_exit;
	gef::Mesh* mesh_exit;

	gef::Scene* model_scene_;
	gef::Mesh* mesh_;
	
	std::vector<MouseCollisionObject*> mouse_collision_lists;

	gef::Vector2 mouse_position_, mouse_position_2;

	// Selection /////////////

	gef::Material* defselectmat;
	gef::Material* selectedmat;
	gef::Material* unavaliblemat;

	int Selection;
	float SelectionCounter;
	int SelectionMax;

	gef::Texture* selectionTexture;

	void Render3DScene(gef::Renderer3D * renderer_3d_);
};