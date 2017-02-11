#ifndef CALIBRATIONDATA_H_
#define CALIBRATIONDATA_H_

#include "kinect_v2.h"

struct ARSCalibrationData
{
	Kinect_v2 KinectSensor;
	int MinDepth;
	int maxDepth;
	int flatDepth;
};



#endif // !CALIBRATIONDATA_H_
