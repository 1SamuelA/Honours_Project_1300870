#ifndef _MESH_APP_H
#define _MESH_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <graphics\mesh.h>
#include <graphics\image_data.h>

#include <camera_object.h>

class TerrainMesh;

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class InputManager;
	class PNGLoader;
}

class TerrainShader;


class MeshApp : public gef::Application
{
public:
	MeshApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SetupCamera();

	void RenderTerrain();

	void ProcessKeyboardInput();
	void ProcessTouchInput();

	gef::InputManager* input_manager_;

	Int32 active_touch_id_;
	gef::Vector2 touch_position_;

	gef::PNGLoader* png_loader_;
	gef::ImageData hieghtmapData;

	CameraObject* camera_0;

	gef::Mesh* CreateCubeMesh();

	TerrainMesh* terrain_mesh_;

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;
	float time_;

	class gef::Renderer3D* renderer_3d_;
	class gef::Mesh* mesh_;
	gef::MeshInstance cube_player_;

	gef::Vector4 camera_eye;
	gef::Vector4 camera_lookat;
	gef::Vector4 camera_up;
	float camera_fov;
	float near_plane;
	float far_plane;

	std::vector<gef::Mesh::Vertex> terrain_verticies;
	std::vector<int> terrain_index;

	float pitch, yaw, roll;


	TerrainShader* terrain_shader_;

};

#endif // _MESH_APP_H
