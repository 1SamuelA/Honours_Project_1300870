#ifndef KINECT_V2_H
#define KINECT_V2_H

#include <Kinect.h>
#include <vector>

// InfraredSourceValueMaximum is the highest value that can be returned in the InfraredFrame.
// It is cast to a float for readability in the visualization code.
#define InfraredSourceValueMaximum static_cast<float>(USHRT_MAX)

// The InfraredOutputValueMinimum value is used to set the lower limit, post processing, of the
// infrared data that we will render.
// Increasing or decreasing this value sets a brightness "wall" either closer or further away.
#define InfraredOutputValueMinimum 0.01f 

// The InfraredOutputValueMaximum value is the upper limit, post processing, of the
// infrared data that we will render.
#define InfraredOutputValueMaximum 1.0f

// The InfraredSceneValueAverage value specifies the average infrared value of the scene.
// This value was selected by analyzing the average pixel intensity for a given scene.
// Depending on the visualization requirements for a given application, this value can be
// hard coded, as was done here, or calculated by averaging the intensity for each pixel prior
// to rendering.
#define InfraredSceneValueAverage 0.08f

/// The InfraredSceneStandardDeviations value specifies the number of standard deviations
/// to apply to InfraredSceneValueAverage. This value was selected by analyzing data
/// from a given scene.
/// Depending on the visualization requirements for a given application, this value can be
/// hard coded, as was done here, or calculated at runtime.
#define InfraredSceneStandardDeviations 3.0f

class Kinect_v2
{
	static const int        cInfraredWidth = 512;
	static const int        cInfraredHeight = 424;

	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

	//static const int        cInfraredWidth = 512;
	//static const int        cInfraredHeight = 424;
	//
public:
	Kinect_v2();
	~Kinect_v2();

	void Init();
	void UpdateIRFeed();
	void UpdateDEFeed();
	
	float** ir_data_2darray;
	int ir_streams_width, ir_streams_height;

	float** de_data_2darray;
	int de_streams_width, de_streams_height;

private:
	IKinectSensor* Sensor;
	// Infrared 
	IInfraredFrameReader* ir_reader_;
	IInfraredFrameSource* ir_frame_source_;

	UINT16* irData;
	byte* irDataConverted;
	RGBQUAD* m_pInfraredRGBX;

	// Depth
	IDepthFrameReader* de_reader_;
	IDepthFrameSource* de_frame_source_;

	UINT16* deData;
	byte* deDataConverted;
	RGBQUAD* m_pDepthRGBX;
	

	BOOLEAN sensor_connected_;

	std::vector<float> *depthValues;
	

	INT64 start_time_;
	DWORD frames_since_update_;
	double freq_;
	int m_nFramesSinceUpdate;
	INT64 last_counter_;
	INT64 m_nNextStatusTime;

	
	

	void ProcessInfrared(INT64 nTime, const UINT16* pBuffer, int nWidth, int nHeight);
	void ProcessDepth(INT64 nTime, const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);
	bool SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);


};


#endif // !KINECT_V2_H
