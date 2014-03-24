#include "Gliw.h"

HINSTANCE hInstance = NULL;
HWND win;

bool running = false;

void setupPixelFormat(HDC deviceContext)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		32,                                     //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		0,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		16,                                     //16 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored

	/*      Set the pixel format to the device context*/
	SetPixelFormat(deviceContext, ChoosePixelFormat(deviceContext, &pfd), &pfd);
}

LRESULT WINAPI DLLWindowProc(HWND win, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HGLRC graphicContext;
	switch(msg)
	{
		case WM_CREATE: //window being created
		{
			HDC deviceContext = GetDC(win);
			setupPixelFormat(deviceContext); //call our pixel format setup function

			/*      Create rendering context and make it current
			*/
			graphicContext = wglCreateContext(deviceContext);
			wglMakeCurrent(deviceContext, graphicContext);

			

			//init();

			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			running = false;
			break;

			// handle other messages.

		default: // anything we dont handle.
			return DefWindowProc(win, msg, wParam, lParam);
	}

	return 0; // just in case
}

bool APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			hInstance = hModule;
			break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			gliw::detail::deInit();
			break;
	}
	return true;
}

int gliw::detail::init(int width, int height)
{
	//HWND win;

	RECT windowRect;
	windowRect.left = (long)0;				//set left value to 0
	windowRect.right = (long)width;			//set right value to requested width
	windowRect.top = (long)0;				//set top value to 0
	windowRect.bottom = (long)height;		//set bottom value to requested height

	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DLLWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = TEXT("MyClass");
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if(!RegisterClassEx(&windowClass))
		return 1;

	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	win = CreateWindowEx(0,
		TEXT("MyClass"),
		TEXT("TITLE"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL,
		hInstance, NULL);

	if(!win)
	{
		return 2;
	}

	ShowWindow(win, SW_SHOW);
	UpdateWindow(win);

	running = true;
	return 0;
}

void gliw::detail::deInit(void)
{
	if(running = false)
		return;

	running = false;

	ReleaseDC(win, GetDC(win));
	DestroyWindow(win);
	win = NULL;
}

int gliw::detail::update(void)
{
	if(!running && win == NULL)
		return 0;

	MSG msg;
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;
}

void gliw::detail::swapBuffers(void)
{
	SwapBuffers(GetDC(win));
}

int gliw::detail::getWidth(void)
{
	RECT rect;
	if(GetWindowRect(win, &rect))
	{
		return rect.right - rect.left;
	}
}

int gliw::detail::getHeight(void)
{
	RECT rect;
	if(GetWindowRect(win, &rect))
	{
		return rect.bottom - rect.top;
	}
}