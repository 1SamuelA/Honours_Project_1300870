#ifndef _CAMERA_H_
#define _CAMERA_H_

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

	void Update();
	void GetViewMatrix(gef::Matrix44&);
	void GetBaseViewMatrix(gef::Matrix44&);

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

	float yaw, pitch, roll;

	gef::Vector4 camera_position_;
	gef::Vector4 camera_rotation_;

	gef::Matrix44 m_viewMatrix;
	float m_speed, m_frameTime;;
};

#endif