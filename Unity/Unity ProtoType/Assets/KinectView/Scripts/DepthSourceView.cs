//using UnityEngine;
//using System.Collections;
//using Windows.Kinect;
//
//public enum DepthMode
//{
//	SeparateSourceReaders,
//	MultiSourceReader,
//}
//
//public class DepthSourceView : MonoBehaviour
//{
//	public DepthMode ViewMode = DepthMode.SeparateSourceReaders;
//
//	public GameObject ColorSourceManager;
//	public GameObject DepthSourceManager;
//	public GameObject MultiSourceManager;
//
//	private KinectSensor _Sensor;
//	private CoordinateMapper _Mapper;
//	private Mesh _Mesh;
//	private Vector3[] _Vertices;
//	private Vector2[] _UV;
//	private int[] _Triangles;
//
//	private bool updateTerrain = true;
//
//	// Only works at 4 right now
//	private const int _DownsampleSize = 1;
//	private const double _DepthScale = 1f;
//	private const int _Speed = 50;
//
//	private MultiSourceManager _MultiManager;
//	private ColorSourceManager _ColorManager;
//	private DepthSourceManager _DepthManager;
//
//	void Start()
//	{
//		_Sensor = KinectSensor.GetDefault();
//		if (_Sensor != null)
//		{
//			_Mapper = _Sensor.CoordinateMapper;
//			var frameDesc = _Sensor.DepthFrameSource.FrameDescription;
//
//			// Downsample to lower resolution
//			CreateMesh(100 , 100 );
//
//			if (!_Sensor.IsOpen)
//			{
//				_Sensor.Open();
//			}
//		}
//	}
//
//	void CreateMesh(int width, int height)
//	{
//		_Mesh = new Mesh();
//		GetComponent<MeshFilter>().mesh = _Mesh;
//
//		_Vertices = new Vector3[width * height];
//		_UV = new Vector2[width * height];
//		_Triangles = new int[6 * ((width - 1) * (height - 1))];
//
//		int triangleIndex = 0;
//		for (int y = 0; y < height; y++)
//		{
//			for (int x = 0; x < width; x++)
//			{
//				int index = (y * width) + x;
//
//				_Vertices[index] = new Vector3(x, -y, 0);
//				_UV[index] = new Vector2(((float)x / (float)width), ((float)y / (float)height));
//
//				// Skip the last row/col
//				if (x != (width - 1) && y != (height - 1))
//				{
//					int topLeft = index;
//					int topRight = topLeft + 1;
//					int bottomLeft = topLeft + width;
//					int bottomRight = bottomLeft + 1;
//
//					_Triangles[triangleIndex++] = topLeft;
//					_Triangles[triangleIndex++] = topRight;
//					_Triangles[triangleIndex++] = bottomLeft;
//					_Triangles[triangleIndex++] = bottomLeft;
//					_Triangles[triangleIndex++] = topRight;
//					_Triangles[triangleIndex++] = bottomRight;
//				}
//			}
//		}
//
//		_Mesh.vertices = _Vertices;
//		_Mesh.uv = _UV;
//		_Mesh.triangles = _Triangles;
//		_Mesh.RecalculateNormals();
//	}
//
//	void OnGUI()
//	{
//		GUI.BeginGroup(new Rect(0, 0, Screen.width, Screen.height));
//		GUI.TextField(new Rect(Screen.width - 250 , 10, 250, 20), "DepthMode: " + ViewMode.ToString());
//		GUI.EndGroup();
//	}
//
//	void Update()
//	{
//		if (_Sensor == null)
//		{
//			return;
//		}
//
//		if (Input.GetButtonDown("Fire1"))
//		{
//
////			if (updateTerrain == true) {
////				updateTerrain = false;
////			} else {
////				updateTerrain = true;
////			}
//
//
//
//			if(ViewMode == DepthMode.MultiSourceReader)
//			{
//				ViewMode = DepthMode.SeparateSourceReaders;
//			}
//			else
//			{
//				ViewMode = DepthMode.MultiSourceReader;
//			}
//		}
//
//		float yVal = Input.GetAxis("Horizontal");
//		float xVal = -Input.GetAxis("Vertical");
//
//		//transform.Rotate (xVal, yVal, 0.0f);
//
//		if (updateTerrain == true) 
//		{
//
//			if (ViewMode == DepthMode.SeparateSourceReaders)
//			{
//				if (ColorSourceManager == null)
//				{
//					return;
//				}
//
//				_ColorManager = ColorSourceManager.GetComponent<ColorSourceManager>();
//				if (_ColorManager == null)
//				{
//					return;
//				}
//
//				if (DepthSourceManager == null)
//				{
//					return;
//				}
//
//				_DepthManager = DepthSourceManager.GetComponent<DepthSourceManager>();
//				if (_DepthManager == null)
//				{
//					return;
//				}
//
//				gameObject.GetComponent<Renderer>().material.mainTexture = _ColorManager.GetColorTexture();
//				RefreshData(_DepthManager.GetData(),
//					_ColorManager.ColorWidth,
//					_ColorManager.ColorHeight,100,100);
//			}
//			else
//			{
//				if (ColorSourceManager == null)
//				{
//					return;
//				}
//
//				_ColorManager = ColorSourceManager.GetComponent<ColorSourceManager>();
//				if (_ColorManager == null)
//				{
//					return;
//				}
//
//				if (DepthSourceManager == null)
//				{
//					return;
//				}
//
//				_DepthManager = DepthSourceManager.GetComponent<DepthSourceManager>();
//				if (_DepthManager == null)
//				{
//					return;
//				}
//
//				gameObject.GetComponent<Renderer>().material.mainTexture = _ColorManager.GetColorTexture();
//				RefreshData(_DepthManager.GetData(),
//					_ColorManager.ColorWidth,
//					_ColorManager.ColorHeight,
//					100,100);
//			}
//
//		}
//
//
//	}
//
//	private void RefreshData(ushort[] depthData, int colorWidth, int colorHeight, int terrainWidth, int terrainHeight)
//	{
//		var frameDesc = _Sensor.DepthFrameSource.FrameDescription;
//		ColorSpacePoint[] colorSpace = new ColorSpacePoint[depthData.Length];
//		_Mapper.MapDepthFrameToColorSpace(depthData, colorSpace);
//
//		float Increment_x = 0;
//		float Increment_y = 0;
//
//		Increment_x = (345f - 159f) / (float)terrainWidth;
//		Increment_y = (322f - 106f) / (float)terrainHeight;
//		Debug.Log (Increment_x);
//		Debug.Log ( Increment_y);
//		for (int y = 0; y < terrainHeight; y ++)
//		{
//			for (int x = 0; x < terrainWidth; x ++)
//			{
//
//				float a = 106f + (Increment_y * (float)y);
//				float b = 159f + (Increment_x * (float)x);
//
//				int bigIndex =  (int)((a * terrainHeight) + b);
//				int smallIndex = (int)((y * terrainHeight) + x);
//
//				if ((y == (terrainHeight )) && (x == (terrainWidth ))) {
//
//					Debug.Log ((int)((a * terrainHeight) + b));
//					Debug.Log ((int)((y * terrainHeight) + x));
//				
//				}
//
//
//				double avg = depthData[bigIndex];//GetAvg(depthData, x, y, frameDesc.Width, frameDesc.Height);
//				avg = avg * 0.01;
//
//				_Vertices [smallIndex].z = 0f;
//
//				// Update UV mapping with CDRP
//				var colorSpacePoint = colorSpace[(y * frameDesc.Width) + x];
//				_UV[smallIndex] = new Vector2(106f +(colorSpacePoint.X / (float)(colorWidth*terrainHeight)), 159f + (colorSpacePoint.Y / (float)(colorHeight*terrainHeight)));
//			}
//		}
//
//		_Mesh.vertices = _Vertices;
//		_Mesh.uv = _UV;
//		_Mesh.triangles = _Triangles;
//		_Mesh.RecalculateNormals();
//	}
//
//	private double GetAvg(ushort[] depthData, int x, int y, int width, int height)
//	{
//		double sum = 0.0;
//
//		for (int y1 = y; y1 < y + 4; y1++)
//		{
//			for (int x1 = x; x1 < x + 4; x1++)
//			{
//				int fullIndex = (y1 * width) + x1;
//
//				if (depthData[fullIndex] == 0)
//					sum += 4500;
//				else
//					sum += depthData[fullIndex];
//
//			}
//		}
//
//		return sum / 16;
//	}
//
//	void OnApplicationQuit()
//	{
//		if (_Mapper != null)
//		{
//			_Mapper = null;
//		}
//
//		if (_Sensor != null)
//		{
//			if (_Sensor.IsOpen)
//			{
//				_Sensor.Close();
//			}
//
//			_Sensor = null;
//		}
//	}
//}




using UnityEngine;
using System.Collections;
using Windows.Kinect;

public enum DepthViewMode
{
    SeparateSourceReaders,
    MultiSourceReader,
}

public class DepthSourceView : MonoBehaviour
{
    public DepthViewMode ViewMode = DepthViewMode.SeparateSourceReaders;
    
    public GameObject ColorSourceManager;
    public GameObject DepthSourceManager;
    public GameObject MultiSourceManager;
    
    private KinectSensor _Sensor;
    private CoordinateMapper _Mapper;
    private Mesh _Mesh;
    private Vector3[] _Vertices;
    private Vector2[] _UV;
    private int[] _Triangles;

	private bool updateTerrain = true;
    
    // Only works at 4 right now
    private const int _DownsampleSize = 4;
    private const double _DepthScale = 0.05f;
    private const int _Speed = 50;
    
    private MultiSourceManager _MultiManager;
    private ColorSourceManager _ColorManager;
    private DepthSourceManager _DepthManager;

	int MeshWidth = 50;
	int MeshHeight = 50;

    void Start()
    {
        _Sensor = KinectSensor.GetDefault();
        if (_Sensor != null)
        {
            _Mapper = _Sensor.CoordinateMapper;
            var frameDesc = _Sensor.DepthFrameSource.FrameDescription;

            // Downsample to lower resolution
			CreateMesh(MeshWidth, MeshHeight);


			this.transform.position = new Vector3 (-MeshWidth/2, 0, MeshHeight/2);

            if (!_Sensor.IsOpen)
            {
                _Sensor.Open();
            }
        }
    }

    void CreateMesh(int width, int height)
    {
        _Mesh = new Mesh();
		_Mesh.name = "Terrain";
        GetComponent<MeshFilter>().mesh = _Mesh;

        _Vertices = new Vector3[width * height];
        _UV = new Vector2[width * height];
        _Triangles = new int[6 * ((width - 1) * (height - 1))];

        int triangleIndex = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = (y * width) + x;

                _Vertices[index] = new Vector3(x, -y, 0);
                _UV[index] = new Vector2(((float)x / (float)width), ((float)y / (float)height));

                // Skip the last row/col
                if (x != (width - 1) && y != (height - 1))
                {
                    int topLeft = index;
                    int topRight = topLeft + 1;
                    int bottomLeft = topLeft + width;
                    int bottomRight = bottomLeft + 1;

                    _Triangles[triangleIndex++] = topLeft;
                    _Triangles[triangleIndex++] = topRight;
                    _Triangles[triangleIndex++] = bottomLeft;
                    _Triangles[triangleIndex++] = bottomLeft;
                    _Triangles[triangleIndex++] = topRight;
                    _Triangles[triangleIndex++] = bottomRight;
                }
            }
        }

        _Mesh.vertices = _Vertices;
        _Mesh.uv = _UV;
        _Mesh.triangles = _Triangles;
        _Mesh.RecalculateNormals();
    }
    
    void OnGUI()
    {
        GUI.BeginGroup(new Rect(0, 0, Screen.width, Screen.height));
        GUI.TextField(new Rect(Screen.width - 250 , 10, 250, 20), "DepthMode: " + ViewMode.ToString());
        GUI.EndGroup();
    }

    void Update()
    {
        if (_Sensor == null)
        {
            return;
        }
        
//        if (Input.GetButtonDown("Fire1"))
//        {
//
//			if (updateTerrain == true) {
//				updateTerrain = false;
//			} else {
//				updateTerrain = true;
//			}
//
//
//
//            if(ViewMode == DepthViewMode.MultiSourceReader)
//            {
//                ViewMode = DepthViewMode.SeparateSourceReaders;
//            }
//            else
//            {
//                ViewMode = DepthViewMode.MultiSourceReader;
//            }
//        }
        
        float yVal = Input.GetAxis("Horizontal");
        float xVal = -Input.GetAxis("Vertical");

		//transform.Rotate (xVal, yVal, 0.0f);
        
		if (updateTerrain == true) 
		{
			
			if (ViewMode == DepthViewMode.SeparateSourceReaders)
			{
				if (ColorSourceManager == null)
				{
					return;
				}

				_ColorManager = ColorSourceManager.GetComponent<ColorSourceManager>();
				if (_ColorManager == null)
				{
					return;
				}

				if (DepthSourceManager == null)
				{
					return;
				}

				_DepthManager = DepthSourceManager.GetComponent<DepthSourceManager>();
				if (_DepthManager == null)
				{
					return;
				}

				gameObject.GetComponent<Renderer>().material.mainTexture = _ColorManager.GetColorTexture();
				RefreshData(_DepthManager.GetData(),
					_ColorManager.ColorWidth,
					_ColorManager.ColorHeight);
			}
			else
			{
				if (MultiSourceManager == null)
				{
					return;
				}

				_MultiManager = MultiSourceManager.GetComponent<MultiSourceManager>();
				if (_MultiManager == null)
				{
					return;
				}

				gameObject.GetComponent<Renderer>().material.mainTexture = _MultiManager.GetColorTexture();

				RefreshData(_MultiManager.GetDepthData(),
					_MultiManager.ColorWidth,
					_MultiManager.ColorHeight);
			}

		}

        
    }
    
    private void RefreshData(ushort[] depthData, int colorWidth, int colorHeight)
    {
        var frameDesc = _Sensor.DepthFrameSource.FrameDescription;

		ColorSpacePoint[] colorSpace = new ColorSpacePoint[depthData.Length];

		float increment_x, increment_y;

		increment_x = (345 - 159) / MeshWidth; //frameDesc.Height / MeshHeight;
		increment_y = (322 - 115) / MeshHeight; //frameDesc.Height / MeshHeight;

        

		for (int y = 0; y < MeshHeight; y ++)
        {
			for (int x = 0; x < MeshWidth; x ++)
            {
				int indexX = 159+(int)(x * increment_x);
				int indexY = 115+(int)(y * increment_y);
				int smallIndex = (y * MeshWidth) + x;
				int bigIndex = (indexY * frameDesc.Width) + indexX;

				double avg = depthData[bigIndex];
                


                avg = avg * _DepthScale;
				avg -= 100;
                
				_Vertices[smallIndex].z = (float)avg;
                
				// Update UV mapping with CDRP
				var colorSpacePoint = colorSpace[(y * frameDesc.Width) + x];
				_UV[smallIndex] = new Vector2(115 +(colorSpacePoint.X / (float)(colorWidth*MeshHeight)), 159 + (colorSpacePoint.Y / (float)(colorHeight*MeshHeight)));
			
            }
        }
        

        

		MeshCollider myMC = GetComponent<MeshCollider>();
		_Mesh.vertices = _Vertices;
		_Mesh.uv = _UV;
		_Mesh.triangles = _Triangles;
		_Mesh.RecalculateNormals();
		_Mesh.RecalculateBounds();
		myMC.sharedMesh = _Mesh;

    }

    void OnApplicationQuit()
    {
        if (_Mapper != null)
        {
            _Mapper = null;
        }
        
        if (_Sensor != null)
        {
            if (_Sensor.IsOpen)
            {
                _Sensor.Close();
            }

            _Sensor = null;
        }
    }

	void OnMouseClick()
	{
		Debug.Log ("Clicked");
	}
}

