#include "Form.h"

//defualt constructor-- creates a new instance of Form with defualt parameters
Form::Form()
{
	//init values
	m_fullscreen = m_height = m_width = m_isInitialized = 0;
	m_recievedQuitMessage = false;
}

//initializes the windows form
//width- the width of the form
//height- the height of the form
//fullscreen- whether the app is fullscreen or not
void Form::InitWindowsForm(int width, int height, bool fullscreen, LPCWSTR name)
{
	//stroe the width and height of the window
	m_width = width;
	m_height = height;

	//set initialized to true since it is working now
	WNDCLASSEX wndClass;
	DEVMODE screenSettings;

	//get the handle instance of this window
	m_hinstance = GetModuleHandle(NULL);

	//store the name of the windows form
	m_windowName = name;

	//store the fullscreen status
	m_fullscreen = fullscreen;

	//set up the windows descriptor with defualt settings
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hinstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_windowName;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	//register the newly created windows class
	RegisterClassEx(&wndClass);

	//get the resloution height and width of the cleints desktop window
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x, y;
	//set the screen settings
	if (m_fullscreen)
	{
		m_width = screenWidth;
		m_height = screenHeight;
		//if fullscreen set the size of the window to the size of the desktop
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = screenWidth;
		screenSettings.dmPelsHeight = screenHeight;
		screenSettings.dmBitsPerPel = 0x20;
		screenSettings.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH | DM_BITSPERPEL;

		//change the display settings and set it to fullscreen
		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

		x = y = 0;
	}
	else
	{
		//since it is windowed put the window in the center
		y = (screenHeight - m_height) / 2;
		x = (screenWidth - m_width) / 2;
	}

	//create the window with the screen settings and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_windowName, m_windowName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
		, x, y, m_width, m_height, NULL, NULL, m_hinstance, NULL);

	//show window and then set it as the main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	//init key states
	memset(m_keyStates, 0, sizeof(m_keyStates));


	//set initailized to true
  	m_isInitialized = true;




}

// returns the fullscreen state
bool Form::GetFullScreen()
{
	return m_fullscreen;
}

//returns the width of the window
int Form::GetWidth()
{
	return m_width;
}

//returns the height of the window
int Form::GetHeight()
{
	return m_height;
}

//returns the name of the window
LPCWSTR Form::GetName()
{
	return m_windowName;
}

//returns the hinstance of the window
HINSTANCE Form::GetHinstance()
{
	return m_hinstance;
}

//returns the hwnd of the window
HWND Form::GetHwnd()
{
	return m_hwnd;
}

 LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return GetForm()->MessageHandler(hwnd, uint, wparam, lparam);
	}
}

 




//returns whether this form has been initialized or not
bool Form::IsInitialized()
{
	return m_isInitialized;
}

//closes the window
void Form::Close()
{

} 

//updates the windows form with messages
void Form::Update()
{
	//call process messages
	processMessages();
}

//returns a boolean indicating whether the user has treid to quit or not
bool Form::GetRecievedQuitMessage()
{
	return m_recievedQuitMessage;
}

LRESULT Form::MessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_KEYDOWN: {
		//record the key press
 		m_keyStates[wparam] = true;
		return 0;
	}
	case WM_KEYUP: {
		//record the key state
		m_keyStates[(unsigned int)wparam] = false;
		return 0;
	}
	default:
		return DefWindowProc(hwnd,uint,wparam,lparam);
	}
}

unsigned char Form::GetKeyState(unsigned char keyID)
{
	return m_keyStates[keyID];
}


//processes and dispatches messages to this windows application
void Form::processMessages()
{
	MSG msg;

	//zero the message memeory
	ZeroMemory(&msg, sizeof(MSG));
	
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//translate and dispath the message
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//if the message is the quit message then set recieved quit message to true
		if (msg.message == WM_QUIT)
			m_recievedQuitMessage = true;
	}
}
