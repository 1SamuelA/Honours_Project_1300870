#pragma once

#include <maths\vector4.h>
#include <maths\math_utils.h>
#include <maths/matrix44.h>

class CameraObject
{
public:
	//	Constructor
	CameraObject(gef::Vector4 camera_eye = gef::Vector4(5.0f, 150.0f, 5.0f), gef::Vector4 camera_lookat = gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4 camera_up = gef::Vector4(0.0f, 1.0f, 0.0f), float camera_fov = 45.0f, float near_plane = 0.01, float far_plane = 1000);

	void update();

	//	Getters
	gef::Vector4 inline GetPos() { return camera_pos_; }
	gef::Vector4 inline GetLook() { return camera_lookat_; }
	gef::Vector4 inline GetUp() { return camera_up_; }
	float inline GetFov() { return camera_fov_; }
	float inline GetNear() { return near_plane_; }
	float inline GetFar() { return far_plane_; }

	float inline GetPitch() {return camera_rotation_.x();}
	float inline GetYaw() {return camera_rotation_.y();}
	float inline GetRoll() {return camera_rotation_.z();}

	

	//	Setter
	void inline SetPos(gef::Vector4 camera_pos) { camera_pos_ = camera_pos; }
	void inline SetLook(gef::Vector4 camera_lookat) { camera_lookat_ = camera_lookat; }
	void inline SetUp(gef::Vector4 camera_up) { camera_up_ = camera_up; }
	void inline SetFov(float fov) { camera_fov_ = gef::DegToRad(fov); }
	void inline SetNear(float near) { near_plane_ = float(near); }
	void inline SetFar(float far) { far_plane_ = float(far); }

	void inline SetStaticLookAt(bool StaticLookAt) { static_look_at_ = StaticLookAt; }

	void inline SetPitch(float pitch) { camera_rotation_.set_x(pitch); }
	void inline SetYaw(float yaw) { camera_rotation_.set_y(yaw); }
	void inline SetRoll(float roll) { camera_rotation_.set_z(roll); }

	void inline SetFrameTime(float frame_time) { m_frameTime = frame_time; }

	void MoveForward();
	void MoveBackward();
	void MoveUp();
	void MoveDown();

	void StrafeRight();
	void StrafeLeft();

	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();


private:
	//	Vars
	gef::Vector4 camera_pos_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	gef::Vector4 camera_right_;

	// Pitch, Yaw, Roll
	gef::Vector4 camera_rotation_;

	// Should always be (0,0,1);
	gef::Vector4 camera_forward_;

	float camera_fov_;
	float near_plane_;
	float far_plane_;

	bool static_look_at_;

	float m_speed;
	float m_frameTime;

	gef::Matrix44 idenity_matrix;
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;

};


/*
namespace gef
{
	class Matrix44;
	class Vector4;
}

class Camera
{
public:
	

	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	gef::Vector4 GetPosition();
	gef::Vector4 GetRotation();
	inline gef::Vector4 GetLookAt() { return up; }
	inline gef::Vector4 GetUp() { return lookAt; }
	gef::Vector4 GetRight();

	void Update();

	void SetFrameTime(float);

	void MoveForward();
	void MoveBackward();
	void MoveUpward();
	void MoveDownward();
	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
	void StrafeRight();
	void StrafeLeft();


private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	gef::Vector4 up, position, lookAt;
	float yaw, pitch, roll;

	gef::Vector4 camera_position_;
	gef::Vector4 camera_rotation_;

	gef::Matrix44 m_viewMatrix;
	float m_speed, m_frameTime;;
};

#endif

*/