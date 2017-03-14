#include "hand_collision.h"

HandCollision::HandCollision()
{
}

HandCollision::HandCollision(gef::Vector2 TopLeft, gef::Vector2 BottomRight)
{
	topleft = TopLeft;
	bottomright = BottomRight;
}

HandCollision::HandCollision(gef::Vector4 TopLeftBottomRight )
{
	topleft = gef::Vector2( TopLeftBottomRight.x(), TopLeftBottomRight.y());
	bottomright = gef::Vector2( TopLeftBottomRight.z(), TopLeftBottomRight.w() );
}

bool HandCollision::Collision( gef::Vector2 collisionpoint )
{

	bool CollisionBool = ((collisionpoint.x > topleft.x) && (collisionpoint.x < bottomright.x)) ? true : false;

	if( CollisionBool )
		CollisionBool = ((collisionpoint.y > topleft.y) && (collisionpoint.y < bottomright.y)) ? true : false;
	else
		CollisionBool = false;

	return CollisionBool;
}
