#ifndef CALIBRATION_STATE_H
#define CALIBRATION_STATE_H

#include "State.h"
#include <vector>
#include "game_object.h"
#include "camera_object.h"

#include "ARS_calibration_data.h"
#include <graphics/scene.h>
#include "assets\png_loader.h"
#include "Shaders\terrain_shader.h"
#include "StateManager.h"

#include "graphics/mesh.h"
#include "Shaders\UI_layer_shader.h"



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
	class Keyboard;

}

class TerrainMesh;
class UILayerShader;
class TerrainShader;

class CALIBRATIONstate : public State
{
public:
	CALIBRATIONstate() :
		model_scene_( NULL ) {
		AlreadyInit = false;
	};

	~CALIBRATIONstate() {};

	void init( gef::Platform* platform, ARSCalibrationData* ARSCalibration, Kinect_v2* kinect_sensor_ );
	void cleanup();
	void Update( StateManager* state_manager_, float delta_time, gef::InputManager* inputmanager );
	void Render( class gef::Renderer3D* renderer_3d_, class gef::SpriteRenderer* sprite_renderer_, gef::Font* font );
	void DrawSprite( class gef::SpriteRenderer* sprite_renderer_, gef::Font* font );
	void DrawMesh( class gef::Renderer3D* renderer_3d_ );
	gef::Mesh* CreateSquare();

private:
	Kinect_v2* KinectSensor_;

	//Init Functions
	void initCamera();
	void initMeshes();
	void InitFont();

	gef::Font* font_;
	float delta_time;


	//Init Varibles
	bool AlreadyInit;
	StateManager* state_manager_;
	ARSCalibrationData* ARSCalibration_;

	//Shaders
	TerrainShader* terrain_shader_;
	UILayerShader* UI_shader;

	ARSCalibrationData* ARSCalibration;
	gef::Platform* platform_;

	CameraObject* camera_0;


	//Objects
	gef::Scene* model_scene_;
	gef::Mesh* mesh_;
	gef::Mesh* forground_mesh_;
	gef::MeshInstance cube_player_;
	gef::MeshInstance forground_meshinstance_;

	TerrainMesh* terrain_mesh_;
	TerrainMesh* forground_terrain;

	std::vector<gef::Mesh::Vertex> terrain_verticies;
	std::vector<int> terrain_index;

	//Update Functions
	void HandleInput( gef::InputManager* input_manager_ );
	void UpdateTerrain();
	void UpdateDepthLayer( TerrainMesh*, gef::Mesh* depthLayerMesh, float minDepth, float maxDepth );
	void CleanUpFont();

	void HandleCameraUpdates( const gef::Keyboard* keyboard );

	//Update varibles
	bool updateKinect;
	bool terrain_changed_;
	bool terrain_changed_Calibration;
	bool terrain_shader_active_;
	float fps_;

	int calibration_mode;
	float timer_depth_update;

	//RenderFunctions
	void RenderTerrain( gef::Renderer3D * renderer_3d_ );
	void DrawHUD( gef::SpriteRenderer * sprite_renderer_ );

	bool OutputConfigFile();

};


#endif