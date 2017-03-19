#ifndef _HAND_COLLISION_H_
#define _HAND_COLLISION_H_

#include "maths\vector2.h"
#include "maths\vector4.h"


class HandCollision
{
public:
	HandCollision();
	HandCollision( gef::Vector2 left_top, gef::Vector2 right_bottom );
	HandCollision( gef::Vector4 left_top_right_bottom );

	bool Collision( gef::Vector2 collisionpoint);

private:
	gef::Vector2 topleft;
	gef::Vector2 bottomright;
	

};


#endif