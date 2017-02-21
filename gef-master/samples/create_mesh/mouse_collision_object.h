#ifndef MOUSE_COLLISION_OBJECT_H
#define MOUSE_COLLISION_OBJECT_H

#include <maths/vector2.h>

class MouseCollisionObject
{
public:
	MouseCollisionObject() { ; }
	~MouseCollisionObject() { ; }

	bool Collision(gef::Vector2 mouse_position_);

	void inline SetTopLeft(gef::Vector2 t_l_) { top_left_ = t_l_; }
	void inline SetBottemRight(gef::Vector2 b_r_) { bottom_right_ = b_r_; }

	void inline SetTopLeftAndBottemRight(gef::Vector2 t_l_, gef::Vector2 b_r_) { top_left_ = t_l_; bottom_right_ = b_r_; }

private:
	gef::Vector2 top_left_, bottom_right_;


};


#endif