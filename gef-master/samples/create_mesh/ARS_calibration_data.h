#ifndef CALIBRATIONDATA_H_
#define CALIBRATIONDATA_H_

#include "kinect_v2.h"
#include "maths\vector4.h"
#include "maths\vector2.h"

struct ARSCalibrationData
{
	//Kinect_v2 KinectSensor;
	int MinDepth;
	int maxDepth;

	int ForgroundMinDepth;
	int ForgroundMaxDepth;

	gef::Vector4 rotation;
	gef::Vector4 LeftRightTopBottom;

	gef::Vector4 Image_LeftRightTopBottom;
};



#endif // !CALIBRATIONDATA_H_
