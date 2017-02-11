#include "game_object.h"

GameObject::GameObject()
{
}

void GameObject::gameObjectInit()
{




}

void GameObject::gameObjectUpdate(float delta_time_)
{
}

void GameObject::gameObjectUpdateTransform()
{
	game_object_transform_.SetIdentity();

	// scale
	gef::Matrix44 scale_matrix;
	scale_matrix.Scale(game_object_scale_);

	// rotation

	// x rotation
	gef::Matrix44 rot_x_matrix;
	rot_x_matrix.RotationX(game_object_rotation_.x());

	// y rotation
	gef::Matrix44 rot_y_matrix;
	rot_y_matrix.RotationY(game_object_rotation_.y());

	// z rotation
	gef::Matrix44 rot_z_matrix;
	rot_z_matrix.RotationZ(game_object_rotation_.z());

	// Set translation
	transform_.SetTranslation(game_object_position_);

	transform_ = transform_ * scale_matrix * rot_x_matrix * rot_y_matrix * rot_z_matrix;
}

