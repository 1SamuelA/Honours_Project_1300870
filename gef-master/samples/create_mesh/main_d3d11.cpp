#include <platform/d3d11/system/platform_d3d11.h>
#include "mesh_app.h"
#include <random>
#include <time.h>

unsigned int sceLibcHeapSize = 128*1024*1024;	// Sets up the heap area size as 128MiB.

void GetDesktopResolution( int& horizontal, int& vertical )
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect( hDesktop, &desktop );
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	
	int hoz, vert;

	GetDesktopResolution( hoz, vert );

	
	// initialisation
	gef::PlatformD3D11 platform(hInstance, hoz, vert, false, true);

	srand(time(NULL));

	MeshApp myApp(platform);
	myApp.Run();

	return 0;
}