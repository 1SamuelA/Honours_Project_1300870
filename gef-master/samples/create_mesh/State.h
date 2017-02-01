#pragma once

#include "StateManager.h"
#include "ARS_calibration_data.h"

namespace gef
{
	class Platform;
	class Renderer3D;
	class SpriteRenderer;
	class Font;
	class InputManager;
}

class State
{
public:
	State() {};

	virtual ~State() {};

	virtual void init(gef::Platform* platform, ARSCalibrationData ARS_calibration_data ) = 0;
	virtual void cleanup() = 0;
	virtual void Update(StateManager* state_manchine,float delta_time, gef::InputManager* input_manager_) = 0;
	virtual void Render(gef::Renderer3D* renderer_3d_, gef::SpriteRenderer* sprite_renderer_, gef::Font* font) = 0;
	virtual void DrawSprite(gef::SpriteRenderer* sprite_renderer_, gef::Font* font) = 0;
	virtual void DrawMesh(gef::Renderer3D* renderer_3d_) = 0;

protected:
	float delta_time_;

};