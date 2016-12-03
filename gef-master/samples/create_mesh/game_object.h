#pragma once

#include "graphics\mesh_instance.h"


class GameObject : public gef::MeshInstance
{
public:
	GameObject();
	
	void gameObjectInit();
	void gameObjectUpdate(float delta_time_);
	inline gef::MeshInstance*  renderMeshInstance() { return (gef::MeshInstance*)this; }

	//Getters
	inline gef::Vector4 getGameObjectPosition() { return game_object_position_; }
	inline gef::Vector4 getGameObjectRotation() { return game_object_rotation_; }
	inline gef::Vector4 getGameObjectScale() { return game_object_scale_; }
	inline gef::Vector4 getGameObjectVelocity() { return game_object_volcity_; }
	inline gef::Vector4 getGameObjectRotationVelocity() { return game_object_rotation_velocity_; }
	
	// Setters
	inline void setGameObjectPosition(gef::Vector4 pos) { game_object_position_ = pos; }
	inline void setGameObjectRotation(gef::Vector4 rot) { game_object_rotation_ = rot; }
	inline void setGameObjectScale(gef::Vector4 sca) { game_object_scale_ = sca; }
	inline void setGameObjectVelocity(gef::Vector4 vol) { game_object_volcity_ = vol; }
	inline void setGameObjectRotationVelocity(gef::Vector4 rot_vol) { game_object_rotation_velocity_ = rot_vol; }


protected:
	inline void setMeshInstanceTransform() { set_transform(game_object_transform_); }

	
	void gameObjectUpdateTransform();

	

	gef::Matrix44 game_object_transform_;

	gef::Vector4 game_object_volcity_;
	gef::Vector4 game_object_rotation_velocity_;

	gef::Vector4 game_object_position_;
	gef::Vector4 game_object_rotation_;
	gef::Vector4 game_object_scale_;


		
};