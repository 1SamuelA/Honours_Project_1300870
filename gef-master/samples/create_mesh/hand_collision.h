#ifndef _HAND_COLLISION_H_
#define _HAND_COLLISION_H_

#include "maths\vector2.h"
#include "maths\vector4.h"
#include "graphics\sprite.h"

class HandCollision : public gef::Sprite
{
public:
	HandCollision();
	HandCollision(int mode, gef::Vector2 LT_OR_XY, gef::Vector2 RB_OR_WH );
	HandCollision( int mode, int L_OR_X, int T_OR_Y, int R_OR_W, int B_OR_H );
	HandCollision(int mode, gef::Vector4 LTRB_OR_XYWH );

	bool Collision( gef::Vector2 collisionpoint);

private:
	gef::Vector2 topleft;
	gef::Vector2 bottomright;
	

};


#endif