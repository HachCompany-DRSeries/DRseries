
#include "ImageProcessor.h"

#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
#include <stdio.h>
#include <process.h>

HBITMAP g_hBitmap;
BITMAP g_bmp;
HWND hwnd;
char *textWindowTitle = "Output";
HWND textWindow;

// This function repaints the image window
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static HDC hMemDC = NULL;
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch(msg)
	{
		case WM_CREATE:
			return 0;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			if (hMemDC == NULL)
				hMemDC = CreateCompatibleDC(hdc);
			if(!hdc)
				return -1;

			SelectObject(hMemDC, g_hBitmap);
			BitBlt( hdc, 0, 0, g_bmp.bmWidth, g_bmp.bmHeight, hMemDC, 0, 0, SRCCOPY );
			EndPaint(hwnd, &ps);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// This function contains the overhead necessary to display each image within a specific time frame
static DWORD WINAPI ImageLoop( void * arg )
{
	ImageProcessor processor;
	CImage *tempColorImage;
	CTimeDate clock;
	long border, title;
	HDC HDC_temp;
	BITMAPINFO bmpInfo;
	int imageWidth, imageHeight;
	unsigned char *screenBitmap_bmBits;
	double startTime;
	double timeToStarNextLoop;
	double remainingTime;
	const double framesPerSecond = 1; // frames per second (feel free to change this)
	const double timePerLoop = 1.0 / framesPerSecond;
	
	clock.StartTimer();
	startTime = clock.GetTimer();

	// Load the first image in a directory and get its width and heigth
	// THE DIRECTORY OF IMAGES MUST BE SPECIFIED HERE!!!!
	if (!processor.Setup("../../images", imageWidth, imageHeight))
	{
		printf("\n\n\n");
		return 0;
	}
	
	// Set the dimentions of the window that we will display the images in.
	// This may need to be changed depending on which operating system you're on.
	border = GetSystemMetrics( SM_CXSIZEFRAME );
	title = GetSystemMetrics( SM_CYMENU );
	
	HDC_temp = CreateCompatibleDC(NULL);
	if (HDC_temp == NULL)		/* Error */
	{
		printf("\n\n\n");
		return 0;
	}
	
	/* CreateDIBSection needs a bitmap header structure with width, height, etc. to work */
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = imageWidth;
	bmpInfo.bmiHeader.biHeight = imageHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	
	tempColorImage = new CImage(imageWidth,imageHeight,24);

	/* CreateDIBSection returns an HBITMAP (needed for display) and gets information about where the
		pixel bits are located */
	g_hBitmap = CreateDIBSection( HDC_temp, (const BITMAPINFO*)&bmpInfo,
			DIB_RGB_COLORS, (void**)tempColorImage->GetTImagePtr()->ppMatrix, NULL, 0);

	delete tempColorImage;
	
	if ( g_hBitmap == NULL )	/* Error */
	{
		DeleteDC(HDC_temp);		/* No longer needed -- avoid memory leaks */
		printf("\n\n\n");
		return 0;
	}
	
	DeleteDC(HDC_temp);		/* No longer needed -- avoid memory leaks */
	GetObject(g_hBitmap, sizeof(g_bmp), &g_bmp);		// Fill in g_bmp structure for display
	
	// Line up the two windows and make sure they are visible.
	textWindow = FindWindow(NULL, textWindowTitle);
	SetWindowPos(textWindow, NULL, 50, 50, 0, 0, SWP_NOSIZE);
	SetWindowPos( hwnd, NULL, 50, 400, imageWidth + (2 * border),
		imageHeight + (2 * border) + title + 6, SWP_SHOWWINDOW);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	// Get a pointer to the screen pixel array were the images need to be placed.
	screenBitmap_bmBits = (unsigned char *)g_bmp.bmBits;
	
	timeToStarNextLoop = startTime;
	while (processor.ProcessNextFrame(screenBitmap_bmBits))
	{
		InvalidateRect( hwnd, NULL, FALSE );
		PostMessage( hwnd, WM_PAINT, 0, 0 ); // repaint the window
		// To wait until the window has been repainted before continuing, uncomment the next line.
//		UpdateWindow( hwnd );
		
		// Make sure we're displaying frames at the specified rate
		timeToStarNextLoop += timePerLoop;
		remainingTime = timeToStarNextLoop - clock.GetTimer();
		if (remainingTime > 0)
			Sleep( remainingTime * 1000 );
	}

	// Draw the last frame
	InvalidateRect( hwnd, NULL, FALSE );
	PostMessage( hwnd, WM_PAINT, 0, 0 ); // repaint the window
	
	printf("Program Run Time: %f\n", clock.GetTimer());
	printf("Done.\n");
	return 0;
}


int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow )
{
	static const char lpszClassName[] = "Bitmap Test";
	WNDCLASSEX wndclass;
	MSG msg;
	
	ZeroMemory(&wndclass, sizeof(wndclass));
	wndclass.cbSize = sizeof(wndclass);
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInst;
	wndclass.hIcon = (HICON)LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpszClassName;
	wndclass.hIconSm = (HICON)LoadIcon(NULL, IDI_APPLICATION);
	
	// Open a console window
	FreeConsole();
	if ( !AllocConsole() )		/* Unable to start debug console */
		return 0;
	SetConsoleTitle(textWindowTitle);
	/* You can install a handler procedure to catch events like control-C and clicking the "close"
		button on the debug window with the system call SetConsoleCtrlHandler()
	*/
	freopen("CONOUT$", "wta+", stdout);

	if(!RegisterClassEx(&wndclass))
		return -1;
	
	hwnd = CreateWindow( lpszClassName, __argv[1], WS_OVERLAPPEDWINDOW, 0,
				0, 320+8, 240+34, NULL, NULL, hInst, NULL );
	if(!hwnd)
		return -1;

	_beginthreadex( NULL, 0, (unsigned int (__stdcall *)(void *))ImageLoop, __argv[1], 0, NULL );

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

