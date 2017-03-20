#include "hand_collision.h"

HandCollision::HandCollision()
{

}

HandCollision::HandCollision(int mode  ,gef::Vector2 LT_OR_XY, gef::Vector2 RB_OR_WH )
{

	if( mode == 0 )
	{
		topleft = LT_OR_XY;
		bottomright = RB_OR_WH;
	}
	else if( mode == 1 )
	{
		topleft = gef::Vector2( LT_OR_XY.x - RB_OR_WH.x, LT_OR_XY.y - RB_OR_WH.y );
		bottomright = gef::Vector2( LT_OR_XY.x + RB_OR_WH.x, LT_OR_XY.y + RB_OR_WH.y );
	}
	
}

HandCollision::HandCollision( int mode , int L_OR_X, int T_OR_Y, int R_OR_W, int B_OR_H )
{

	if( mode == 0 )
	{
		topleft = gef::Vector2( L_OR_X, T_OR_Y );
		bottomright = gef::Vector2( R_OR_W, B_OR_H );
	}
	else if( mode == 1 )
	{
		topleft = gef::Vector2( L_OR_X - (R_OR_W/2), T_OR_Y - (B_OR_H/2) );
		bottomright = gef::Vector2( L_OR_X + (R_OR_W/2), T_OR_Y + (B_OR_H/2) );
	}

}

HandCollision::HandCollision( int mode ,gef::Vector4 LTRB_OR_XYWH )
{
	topleft = gef::Vector2( LTRB_OR_XYWH.x(), LTRB_OR_XYWH.y());
	bottomright = gef::Vector2( LTRB_OR_XYWH.z(), LTRB_OR_XYWH.w() );
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
