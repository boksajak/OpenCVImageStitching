#include "common.h"

// Include OpenGL Renderer
#include "GLRenderer.h"

// Include ImageStitching
#include "ImageStitching.h"


GLRenderer glRenderer;

void ProcessInput(UINT message, WPARAM wParam, LPARAM lParam);

// http://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution
// Get the horizontal and vertical screen sizes in pixel (of primary monitor)
void GetDesktopResolution(int& screenWidth, int& screenHeight, int& workAreaWidth, int& workAreaHeight)
{
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect, NULL);

	workAreaWidth = rect.right;
	workAreaHeight = rect.bottom;     
}

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}

std::vector<std::string> LoadImageNamesToStitch() {
	std::vector<std::string> result;

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
	char sDir[2048];
	//strcpy(sDir, ExePath().c_str());

	GetCurrentDirectory(2048, sDir);

	strcat(sDir, "\\assets");

    //Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        TRACE_ERROR("Path not found: "<<sDir<<"\n");
        return result;
    }

    do
    {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            //Is the entity a File or Folder?
            if((fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				result.push_back(sPath);
				TRACE_LOADER("Found input file: "<<sPath<<"\n");
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return result;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR cmdLine, INT iCmdShow)
{

	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;

	wndClass.style          = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc    = WndProc;
	wndClass.cbClsExtra     = 0;
	wndClass.cbWndExtra     = 0;
	wndClass.hInstance      = hInstance;
	wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName   = NULL;
	wndClass.lpszClassName  = "Physics";

	RegisterClass(&wndClass);

	// Get screen resolution
	int screenWidth, screenHeight, workAreaWidth, workAreaHeight;
	GetDesktopResolution(screenWidth, screenHeight, workAreaWidth, workAreaHeight);

	// Compute optimal window position on screen
	int left = (workAreaWidth - WIDTH - 550) / 3;
	int top = (workAreaHeight - HEIGHT) / 3;

	hWnd = CreateWindow(
		"Physics",   // window class name
		"Physics",  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		left,            // initial x position
		top,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters


	// compute window size required to desired client area size
	RECT clientRect;
	clientRect.top = 0;
	clientRect.left = 0;
	clientRect.right = WIDTH;
	clientRect.bottom = HEIGHT;

	DWORD dwStyle = GetWindowLongPtr( hWnd, GWL_STYLE ) ;
	DWORD dwExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE ) ;

	if (!AdjustWindowRectEx( &clientRect, dwStyle, false, dwExStyle )) {
		return 1;
	}

	SetWindowPos( hWnd, NULL, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SWP_NOZORDER | SWP_NOMOVE ) ;

	// Move console window to nice position right of main window
#ifdef USE_CONSOLE
	consoleInst.move(left + clientRect.right - clientRect.left, top, 550, HEIGHT);
#endif

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// Do image stitching
	std::vector<std::string> inputImages = LoadImageNamesToStitch();

	ImageStitching::StitchImages(inputImages);

	glRenderer.Initialize(hWnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		glRenderer.Render();
	}

	return msg.wParam;
}  

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;

	switch(message)
	{
	case WM_PAINT:
		return 0;
	case WM_MOVE:
		hdc = GetDC( hWnd );
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MOUSEWHEEL:
		ProcessInput(message, wParam, lParam);
		return 0;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} 

void ProcessInput(UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	default:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEWHEEL:
		break;
	}

	glRenderer.ProcessInput(message, wParam, lParam);
}
