#include "camera_object.h"
#include <maths\matrix44.h>
#include <maths\vector4.h>
#include <maths\quaternion.h>
#include <maths\math_utils.h>
#include <math.h>

#define CAMERA_FIRST_PERSON 0
#define CAMERA_TOP_DOWN 1
#define CAMERA_TRACKING 1

CameraObject::CameraObject(gef::Vector4 camera_eye, gef::Vector4 camera_forward, gef::Vector4 camera_up, float camera_fov, float near_plane, float far_plane)
{

	camera_pos_ = camera_eye;
	camera_forward_ = camera_forward;
	camera_lookat_ = camera_pos_ + camera_forward;
	camera_up_ = camera_up;
	camera_fov_ = gef::DegToRad(camera_fov);
	near_plane_ = near_plane;
	far_plane_ = far_plane;

	static_look_at_ = false;
	camera_rotation_ = gef::Vector4(-45.0f, -90.0f, 0.0f);
}



void CameraObject::update()
{
	

	float cosR, cosP, cosY;	//temp values for sin/cos from
	float sinR, sinP, sinY;
	
	cosY = cosf(camera_rotation_.y()*(FRAMEWORK_PI / 180.0));
	cosP = cosf(camera_rotation_.x()*(FRAMEWORK_PI / 180.0));
	cosR = cosf(camera_rotation_.z()*(FRAMEWORK_PI / 180.0));
	sinY = sinf(camera_rotation_.y()*(FRAMEWORK_PI / 180.0));
	sinP = sinf(camera_rotation_.x()*(FRAMEWORK_PI / 180.0));
	sinR = sinf(camera_rotation_.z()*(FRAMEWORK_PI / 180.0));

	float forwardx, forwardy, forwardz;

	forwardx = sinY * cosP;
	forwardy = sinP;
	forwardz = cosP * -cosY;

	camera_forward_ = gef::Vector4(forwardx, forwardy, forwardz);
	camera_forward_.Normalise();

	


	camera_lookat_ = camera_forward_ + camera_pos_;
	// Setup Up

	float upx, upy, upz;

	upx = -cosY * sinR - sinY * sinP * cosR;
	upy = cosP * cosR;
	upz = -sinY * sinR - sinP * cosR * -cosY;

	camera_up_ = gef::Vector4(upx, upy, upz);

	camera_up_.Normalise();

	/*
	void Camera::Update()
	{

	gef::Matrix44 rotationMatrix;


	// Setup the vector that points upwards.
	up = gef::Vector4(0.0f, 1.0, 0.0, 1.0f);

	// Setup the position of the camera in the world.
	m_positionX = camera_position_.x();
	m_positionY = camera_position_.y();
	m_positionZ = camera_position_.z();

	// Setup where the camera is looking by default.
	lookAt = gef::Vector4(0.0, 0.0, 1.0f, 1.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	float cosR, cosP, cosY;	//temp values for sin/cos from
	float sinR, sinP, sinY;

	cosY = cosf(yaw*3.1415 / 180.0);
	cosP = cosf(pitch*3.1415 / 180.0);
	cosR = cosf(roll*3.1415 / 180.0);
	sinY = sinf(yaw*3.1415 / 180.0);
	sinP = sinf(pitch*3.1415 / 180.0);
	sinR = sinf(roll*3.1415 / 180.0);

	gef::Vector4 forward_ = gef::Vector4(sinY * cosP, sinP, cosP * -cosY);
	forward_.Normalise();



	gef::Matrix44 Rotation_X, Rotation_Y, Rotation_Z;

	Rotation_X.SetIdentity();
	Rotation_Y.SetIdentity();
	Rotation_Z.SetIdentity();

	Rotation_X.RotationX(pitch);
	Rotation_Y.RotationY(pitch);
	Rotation_Z.RotationZ(pitch);

	rotationMatrix = Rotation_X * Rotation_Y * Rotation_Z;

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.


	lookAt = camera_position_ + forward_;

	float upx, upy, upz;

	upx = -cosY * sinR - sinY * sinP * cosR;
	upy = cosP * cosR;
	upz = -sinY * sinR - sinP * cosR * -cosY;

	up = gef::Vector4(upx, upy, upz);

	up.Normalise();
	up;
	// Translate the rotated camera position to the location of the viewer.

	// Finally create the view matrix from the three updated vectors.

	}

	*/


}

/*
Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

Camera::~Camera()
{
	m_speed = 0.5;
	yaw = 45;
}

void Camera::SetFrameTime(float t)
{
	m_frameTime = t;
}

void Camera::SetPosition(float x, float y, float z)
{
	
	
	camera_position_ = gef::Vector4(x, y, z, 1.0f);
}

void Camera::SetRotation(float x, float y, float z)
{
	camera_rotation_ = gef::Vector4(x, y, z);
}

gef::Vector4 Camera::GetPosition()
{
	//XMVECTOR pos;
	//pos = XMVectorSet(m_positionX, m_positionY, m_positionZ, 1.0f);
	
	//position = gef::Vector4(m_positionX, m_positionY, m_positionZ, 1.0f);
	return camera_position_;
}

gef::Vector4 Camera::GetRotation()
{
	gef::Vector4 rot;
	rot = gef::Vector4(m_rotationX, m_rotationY, m_rotationZ, 1.0f);
	return rot;
}



void Camera::MoveForward()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_speed;
	m_positionZ += cosf(radians) * m_speed;

	camera_position_.set_x(m_positionX);
	camera_position_.set_z(m_positionZ);
}


void Camera::MoveBackward()
{
	float radians;

	// Update the backward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;

								// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_speed;
	m_positionZ -= cosf(radians) * m_speed;

	camera_position_.set_x(m_positionX);
	camera_position_.set_z(m_positionZ);
}


void Camera::MoveUpward()
{
	// Update the upward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;

								// Update the height position.
	m_positionY += m_speed;

	camera_position_.set_y(m_positionY);
}


void Camera::MoveDownward()
{
	// Update the downward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;

								// Update the height position.
	m_positionY -= m_speed;

	camera_position_.set_y(m_positionY);
}


void Camera::TurnLeft()
{
	// Update the left turn movement based on the frame time 
	m_speed = m_frameTime * 25.0f;

	// Update the rotation.
	m_rotationY -= m_speed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}
}


void Camera::TurnRight()
{
	// Update the right turn movement based on the frame time
	m_speed = m_frameTime * 25.0f;

	// Update the rotation.
	m_rotationY += m_speed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

}


void Camera::TurnUp()
{
	// Update the upward rotation movement based on the frame time
	m_speed = m_frameTime * 25.0f;

	// Update the rotation.
	m_rotationX -= m_speed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}
}


void Camera::TurnDown()
{
	// Update the downward rotation movement based on the frame time
	m_speed = m_frameTime * 25.0f;

	// Update the rotation.
	m_rotationX += m_speed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}
}

void Camera::StrafeRight()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionZ -= sinf(radians) * m_speed;
	m_positionX += cosf(radians) * m_speed;

	camera_position_.set_x(m_positionX);
	camera_position_.set_z(m_positionZ);

}

void Camera::StrafeLeft()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionZ += sinf(radians) * m_speed;
	m_positionX -= cosf(radians) * m_speed;

	camera_position_.set_x(m_positionX);
	camera_position_.set_z(m_positionZ);
}

*/