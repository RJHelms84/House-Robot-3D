#include "Engine.h"

namespace Batman
{
	Input::Input(HWND hwnd)
	{
		//save window handle
		window = hwnd;

		//create DirectInput object
		DirectInput8Create(
			GetModuleHandle(NULL),		//get this app handle
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,			//unique id
			(void**)&di,				//pointer to di object
			NULL
			);

		//initialise keyboard
		di->CreateDevice(
			GUID_SysKeyboard,			//unique id
			&keyboard,					//keyboard object
			NULL
			);

		//keyboard data format (standard DI)
		keyboard->SetDataFormat(&c_dfDIKeyboard);

		//for this window foreground means
		//use when in FOREGROUND and give up access
		//thereafter; NONEXCLUSIVE
		//means it doesn't stop other apps using it
		keyboard->SetCooperativeLevel(
			window,
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);

		//Get keyboard control
		keyboard->Acquire();

		//clear key array
		memset(keyState, 0, 256);

		//initialise mouse
		di->CreateDevice(
			GUID_SysMouse,		//unique id
			&mouse,				//mouse object
			NULL				//NULL
			);

		//mouse data format (standard DI)
		mouse->SetDataFormat(&c_dfDIMouse);

		//for this window foreground means
		//use when in FOREGROUND and give up access
		//thereafter; NONEXCLUSIVE
		//means it doesn't stop other apps using it
		mouse->SetCooperativeLevel(
			window,
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);

		//get mouse control
		mouse->Acquire();
	}

	Input::~Input()
	{
		mouse->Release();		//release mouse object
		keyboard->Release();	//release keyboard object
		di->Release();			//release direct input object
	}

	void Input::Update()
	{
		//poll state of the keyboard
		keyboard->Poll();

		//get state of keys - buffer and pointer to storage
		if(!SUCCEEDED(
			keyboard->GetDeviceState(256,(LPVOID)&keyState))
			)
		{
			//keyboard device lost, try to re-acquire
			keyboard->Acquire();
		}

		//poll state of the mouse
		mouse->Poll();

		//get state of mouse - buffer and pointer to storage
		if(!SUCCEEDED(
			mouse->GetDeviceState(sizeof(DIMOUSESTATE),&mouseState))
			)
		{
			//mouse device lost, try to re-acquire
			mouse->Acquire();
		}

		//get mouse position on screen (not DirectInput)
		GetCursorPos(&position);

		//get position relative to window origin
		ScreenToClient(window, &position);
	}

	int Input::GetMouseButton(char button)
	{
		//get whether button is pressed
		return (mouseState.rgbButtons[button] & 0x80);
	}
};