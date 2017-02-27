#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//A class that handles window of the app, this class is implemented in singleton style
class Form
{
public:

	//defualt constructor-- creates a new instance of Form with defualt parameters
	Form();
	
public:

	//initializes the windows form
	//width- the width of the form
	//height- the height of the form
	//fullscreen- whether the app is fullscreen or not
	void InitWindowsForm(int width, int height, bool fullscreen,LPCWSTR name);

	// returns the fullscreen state
	bool GetFullScreen();

	//returns the width of the window
	int GetWidth();

	//returns the height of the window
	int GetHeight();


	//returns the name of the window
	LPCWSTR GetName();

	//returns the hinstance of the window
	HINSTANCE GetHinstance();

	//returns the hwnd of the window
	HWND GetHwnd();
	
	//returns whether this form has been initialized or not
	bool IsInitialized();

	//closes the window
	void Close();

	//updates the windows form with messages
	void Update();
	//updates the windows form with messages

	//returns a boolean indicating whether the user has treid to quit or not
	bool GetRecievedQuitMessage();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);

	unsigned char GetKeyState(unsigned char keyID);

	float inline GetPixelWidth() { return 1.0f / m_width; }

	float inline GetPixelHeight() { return 1.0f / m_height; }

private:

	//processes and dispatches messages to this windows application
	void processMessages();

	//a bolean indicating whether this app is fullscreen or not
	bool m_fullscreen;

	//the width of the windows form
	int m_width;

	//the height of the windows form
	int m_height;

	//the window class name
	LPCWSTR m_windowName;

	//the hinstance of this form
	HINSTANCE m_hinstance;

	//the hwnd of the form
	HWND m_hwnd;

	//a bool indicating whther
	bool m_isInitialized;

	//a bool indicating whether the user has requested to quit or not
	bool m_recievedQuitMessage;

	unsigned char m_keyStates[256];


};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Form* formInstance = NULL;
//returns the main form instance
static Form* GetForm()
{
		if (formInstance == NULL)
			formInstance = new Form();
		return formInstance;
}

//the main instance of this form( this class is used as a singleton




