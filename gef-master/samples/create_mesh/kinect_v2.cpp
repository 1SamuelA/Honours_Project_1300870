#include "kinect_v2.h"

Kinect_v2::Kinect_v2()
{
}

Kinect_v2::~Kinect_v2()
{
	if (sensor_connected_)
	{
		Sensor->Close();
	}

}

void Kinect_v2::Init()
{
	GetDefaultKinectSensor(&Sensor);
	
	//Sensor->get_InfraredFrameSource(&ir_frame_source_);
	//ir_frame_source_->OpenReader(&ir_reader_);
	//IFrameDescription* fd;
	//ir_frame_source_->get_FrameDescription(&fd);

	Sensor->get_DepthFrameSource(&de_frame_source_);
	de_frame_source_->OpenReader(&de_reader_);
	IFrameDescription* fde;
	de_frame_source_->get_FrameDescription(&fde);


	if (de_frame_source_ != NULL)
	{
		de_frame_source_->Release();
		de_frame_source_ = NULL;
	}

	depthValues = NULL;
	

	if (sensor_connected_)
	{
		Sensor->Open();
		
	}
	
	m_pInfraredRGBX = new RGBQUAD[cInfraredWidth * cInfraredHeight];

	m_pDepthRGBX = new RGBQUAD[cInfraredWidth * cInfraredHeight];
	
	//ir_reader_->SubscribeFrameArrived;


}

void Kinect_v2::UpdateIRFeed()
{
	if (!ir_reader_)
	{
		return;
	}
	
	IInfraredFrame*  ir_frame_;

	HRESULT hr = ir_reader_->AcquireLatestFrame(&ir_frame_);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = NULL;
		UINT nBufferSize = 0;
		UINT16 *pBuffer = NULL;

		hr = ir_frame_->get_RelativeTime(&nTime);

		if (SUCCEEDED(hr))
		{
			hr = ir_frame_->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&ir_streams_width);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&ir_streams_height);
		}

		if (SUCCEEDED(hr))
		{
			hr = ir_frame_->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		}

		if (SUCCEEDED(hr))
		{
			ProcessInfrared(nTime, pBuffer, ir_streams_width, ir_streams_height);
		}

		if (pFrameDescription != NULL)
		{
			pFrameDescription->Release();
			pFrameDescription = NULL;
		}

		
	}

	if (ir_frame_ != NULL)
	{
		ir_frame_->Release();
		ir_frame_ = NULL;
	}

	unsigned int length;

	//fd->get_LengthInPixels(&length);
	//irData = new UINT16[length];
	//irDataConverted = new byte[length * 4];
	//
	//fd->get_Width(&ir_streams_width);
	//fd->get_Height(&ir_streams_height);

	ir_data_2darray = new float*[ir_streams_height];
	for (int i = 0; i < ir_streams_height; i++)
	{
		ir_data_2darray[i] = new float[ir_streams_width];
	}

	Sensor->get_IsAvailable(&sensor_connected_);




	for (int y = 0; y < ir_streams_height; y++)
	{
		for (int x = 0; x < ir_streams_width; x++)
		{
			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			ir_data_2darray[y][x] = m_pInfraredRGBX[(y*ir_streams_width) + x].rgbBlue;
			
		}
	}

}

void Kinect_v2::ProcessInfrared(INT64 nTime, const UINT16 * pBuffer, int nWidth, int nHeight)
{
	if (!start_time_)
	{
		start_time_ = nTime;
	}

	double fps = 0.0;

	LARGE_INTEGER qpcNow = { 0 };
	if (freq_)
	{
		if (QueryPerformanceCounter(&qpcNow))
		{
			if (last_counter_)
			{
				m_nFramesSinceUpdate++;
				fps = freq_ * m_nFramesSinceUpdate / double(qpcNow.QuadPart - last_counter_);
			}
		}
	}

	WCHAR szStatusMessage[64];
	//StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

	if (SetStatusMessage(szStatusMessage, 1000, false))
	{
		last_counter_ = qpcNow.QuadPart;
		m_nFramesSinceUpdate = 0;
	}

	if (m_pInfraredRGBX && pBuffer && (nWidth == cInfraredWidth) && (nHeight == cInfraredHeight))
	{
		RGBQUAD* pDest = m_pInfraredRGBX;

		// end pixel is start + width*height - 1
		const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);

		while (pBuffer < pBufferEnd)
		{
			// normalize the incoming infrared data (ushort) to a float ranging from 
			// [InfraredOutputValueMinimum, InfraredOutputValueMaximum] by
			// 1. dividing the incoming value by the source maximum value
			float intensityRatio = static_cast<float>(*pBuffer) / InfraredSourceValueMaximum;

			// 2. dividing by the (average scene value * standard deviations)
			intensityRatio /= InfraredSceneValueAverage * InfraredSceneStandardDeviations;

			// 3. limiting the value to InfraredOutputValueMaximum
			intensityRatio = min(InfraredOutputValueMaximum, intensityRatio);

			// 4. limiting the lower value InfraredOutputValueMinimym
			intensityRatio = max(InfraredOutputValueMinimum, intensityRatio);

			// 5. converting the normalized value to a byte and using the result
			// as the RGB components required by the image
			byte intensity = static_cast<byte>(intensityRatio * 255.0f);
			pDest->rgbRed = intensity;
			pDest->rgbGreen = intensity;
			pDest->rgbBlue = intensity;

			++pDest;
			++pBuffer;
		}

		
	}


}

void Kinect_v2::UpdateDEFeed()
{
	if (!ir_reader_)
	{
		return;
	}
	
	IDepthFrame*  de_frame_;

	HRESULT hr = de_reader_->AcquireLatestFrame(&de_frame_);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = NULL;
		UINT nBufferSize = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxDistance = 0;
		UINT16 *pBuffer = NULL;

		hr = de_frame_->get_RelativeTime(&nTime);

		if (SUCCEEDED(hr))
		{
			hr = de_frame_->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&de_streams_width);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&de_streams_height);
		}

		if (SUCCEEDED(hr))
		{
			hr = de_frame_->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		
		if (SUCCEEDED(hr))
		{
			// In order to see the full range of depth (including the less reliable far field depth)
			// we are setting nDepthMaxDistance to the extreme potential depth threshold
			nDepthMaxDistance = USHRT_MAX;

			// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
			hr = de_frame_->get_DepthMaxReliableDistance(&nDepthMaxDistance);

			nDepthMaxDistance = 1000;
		}

		if (SUCCEEDED(hr))
		{
			hr = de_frame_->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		}

		if (SUCCEEDED(hr))
		{
			ProcessDepth(nTime, pBuffer, de_streams_width, de_streams_height, nDepthMinReliableDistance, nDepthMaxDistance);
		}
		if (pFrameDescription != NULL)
		{
			pFrameDescription->Release();
			pFrameDescription = NULL;
		}

		
	}

	if (de_frame_ != NULL)
	{
		de_frame_->Release();
		de_frame_ = NULL;
	}

	unsigned int length;

	//fd->get_LengthInPixels(&length);
	//irData = new UINT16[length];
	//irDataConverted = new byte[length * 4];
	//
	//fd->get_Width(&ir_streams_width);
	//fd->get_Height(&ir_streams_height);

	de_data_2darray = new float*[de_streams_height];
	for (int i = 0; i < de_streams_height; i++)
	{
		de_data_2darray[i] = new float[de_streams_width];
	}

	Sensor->get_IsAvailable(&sensor_connected_);




	for (int y = 0; y < de_streams_height; y++)
	{
		for (int x = 0; x < de_streams_width; x++)
		{
			//ir_data_2darray[x][y] = irData[(y*ir_streams_width) + x];
			int slot = int((y*de_streams_width) + x);
			if( depthValues->at( slot ) > 0.5 )
			{
				depthValues->operator[]( slot ) = depthValues->at( slot ) - 20;

			}
			else
			{

			}
			de_data_2darray[y][x] = depthValues->at(slot);
			
		}
	}

}

void Kinect_v2::ProcessDepth(INT64 nTime, const UINT16 * pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	if (!start_time_)
	{
		start_time_ = nTime;
	}

	double fps = 0.0;

	LARGE_INTEGER qpcNow = { 0 };
	if (freq_)
	{
		if (QueryPerformanceCounter(&qpcNow))
		{
			if (last_counter_)
			{
				m_nFramesSinceUpdate++;
				fps = freq_ * m_nFramesSinceUpdate / double(qpcNow.QuadPart - last_counter_);
			}
		}
	}

	WCHAR szStatusMessage[64];
	//StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

	if (SetStatusMessage(szStatusMessage, 1000, false))
	{
		last_counter_ = qpcNow.QuadPart;
		m_nFramesSinceUpdate = 0;
	}

	if (m_pDepthRGBX && pBuffer && (nWidth == cInfraredWidth) && (nHeight == cInfraredHeight))
	{
		RGBQUAD* pDest = m_pDepthRGBX;

		// end pixel is start + width*height - 1
		const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);

		if (depthValues != NULL)
		{
			delete depthValues;
			depthValues = NULL;
		}

		depthValues = new std::vector<float>;

		while (pBuffer < pBufferEnd)
		{
			USHORT depth = *pBuffer;

			

			// To convert to a byte, we're discarding the most-significant
			// rather than least-significant bits.
			// We're preserving detail, although the intensity will "wrap."
			// Values outside the reliable depth range are mapped to 0 (black).

			// Note: Using conditionals in this loop could degrade performance.
			// Consider using a lookup table instead when writing production code.

			float depthValue = static_cast<FLOAT>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (((float)depth / (float)nMaxDepth) * 256.f) : 0);

			byte intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (((float)depth / (float)nMaxDepth) * 256.f) : 0);
			
			depthValues->push_back(depthValue);

			pDest->rgbRed = intensity;
			pDest->rgbGreen = intensity;
			pDest->rgbBlue = intensity;

			

			++pDest;
			++pBuffer;
		}


	}


}



bool Kinect_v2::SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
{
	INT64 now = GetTickCount64();

	if ((bForce || (m_nNextStatusTime <= now)))
	{
		m_nNextStatusTime = now + nShowTimeMsec;

		return true;
	}

	return false;
}
