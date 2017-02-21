#include "mouse_collision_object.h"

bool MouseCollisionObject::Collision(gef::Vector2 mouse_position_)
{
	bool CollisionBool = true;
	//if (
	//	(mouse_position_.x > top_left_.x) &&
	//	(mouse_position_.y < top_left_.y) &&
	//	(mouse_position_.x < bottom_right_.x) &&
	//	(mouse_position_.y > bottom_right_.y)
	//	)
	//{
	//	return true;
	//}
	CollisionBool = ((mouse_position_.x > top_left_.x) && (mouse_position_.x < bottom_right_.x)) ? true : false;
	
	if (CollisionBool)
	{
		CollisionBool = ((mouse_position_.y > top_left_.y) && (mouse_position_.y < bottom_right_.y)) ? true : false;
	}
	else
		CollisionBool = false;
	
	
	return CollisionBool;
	
	//if ((mouse_position_.x > top_left_.x) && (mouse_position_.x < bottom_right_.x))
	//{
	//
	//}


	//else
	//{
	//	return false;
	//}
}
