#pragma comment(linker, "/subsystem:windows")

#include <windowsx.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

HDC deviceContext;
GLuint vbo;

void init()
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	static const GLfloat data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	SwapBuffers(deviceContext);
}

//function to set the pixel format for the device context
void SetupPixelFormat()
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

//      Windows Event Procedure Handler
LRESULT CALLBACK WndProc(HWND win, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HGLRC graphicContext;

	switch (msg)
	{
		case WM_CREATE: //window being created
		{
			deviceContext = GetDC(win);  //get current windows device context
			SetupPixelFormat(); //call our pixel format setup function

			/*      Create rendering context and make it current
			*/
			graphicContext = wglCreateContext(deviceContext);
			wglMakeCurrent(deviceContext, graphicContext);

			glewInit();

			std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
			std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "Graphic Card: " << glGetString(GL_RENDERER) << std::endl;

			init();

			return 0;
		}
		case WM_CLOSE:  //window is closing
		{
			/*      Deselect rendering context and delete it*/
			wglMakeCurrent(deviceContext, NULL);
			wglDeleteContext(graphicContext);

			/*      Send quit message to queue*/
			PostQuitMessage(0);

			return 0;
		}
		case WM_SIZE:
		{
			/*      Retrieve width and height*/
			int height = HIWORD(lParam);
			int width = LOWORD(lParam);

			/*      Don't want a divide by 0*/
			if (height == 0)
			{
				height = 1;
			}

			/*      Reset the viewport to new dimensions*/
			glViewport(0, 0, width, height);

			return 0;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}

			std::cout << (char)MapVirtualKey(wParam, MAPVK_VK_TO_CHAR) << " (" << wParam << ") is down" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_KEYUP:
		{
			std::cout << (char)MapVirtualKey(wParam, MAPVK_VK_TO_CHAR) << " is up (" << wParam << ")" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			std::cout << "Mouse x: " << GET_X_LPARAM(lParam) << " y: " << GET_Y_LPARAM(lParam) << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			std::cout << "Left Mouse Button down" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			std::cout << "Left Mouse Button up" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			std::cout << "Right Mouse Button down" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			std::cout << "Right Mouse Button up" << std::endl;
			SetFocus(win);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			std::cout << "Mouse Scroll wheel; MouseKeyDown: " << GET_KEYSTATE_WPARAM(wParam) << " delta: " << GET_WHEEL_DELTA_WPARAM(wParam) / 120 << std::endl;
			SetFocus(win);
			return 0;
		}
		default:
			return (DefWindowProc(win, msg, wParam, lParam));
	}
}

void createConsole()
{
	int hConHandle;
	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console

	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	createConsole();
	WNDCLASSEX windowClass;					//window class
	HWND win;								//window handle
	MSG msg;								//message
	bool done = false;						//flag for completion of app
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;	//windows style
	RECT windowRect;

	/*      Screen/display attributes*/
	int width = 800;
	int height = 600;

	windowRect.left = (long)0;				//set left value to 0
	windowRect.right = (long)width;			//set right value to requested width
	windowRect.top = (long)0;				//set top value to 0
	windowRect.bottom = (long)height;		//set bottom value to requested height

	/*      Fill out the window class structure*/
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = TEXT("MyClass");
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	/*      Register window class*/
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	/*      Class registerd, so now create our window*/
	win = CreateWindowEx(NULL, TEXT("MyClass"),	//class name
		TEXT("OpenGL Robot"),					//app name
		dwStyle |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,									//x and y coords
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,		//width, height
		NULL,									//handle to parent
		NULL,									//handle to menu
		hInstance,								//application instance
		NULL);									//no xtra params

	/*      Check if window creation failed (hwnd = null ?)*/
	if (!win)
	{
		return 0;
	}

	ShowWindow(win, SW_SHOW);					//display window
	UpdateWindow(win);							//update window

	/*      Main message loop*/
	while (!done)
	{
		PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);

		if (msg.message == WM_QUIT)				//did we receive a quit message?
		{
			done = true;
		}
		else
		{
			render();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}