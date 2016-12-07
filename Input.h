#pragma once

#include "Engine.h"

namespace Batman
{
	class Input
	{
	private:
		HWND window;
		IDirectInput8 *di; //direct input device
		IDirectInputDevice8 *keyboard; //keyboard
		char keyState[256];		//keystate array
		IDirectInputDevice8 *mouse;
		DIMOUSESTATE mouseState;
		POINT position;		//x,y cursor position
	public:
		Input(HWND window);	//constructor
		virtual ~Input();
		void Update();

		char GetKeyState(int key) { return keyState[key]; }
		long GetMousePosX() { return position.x; }
		long GetMousePosY() { return position.y; }
		int GetMouseButton(char button);
		long GetMouseDeltaX() { return mouseState.lX; }
		long GetMouseDeltaY() { return mouseState.lY; }
		long GetMouseDeltaWheel() { return mouseState.lZ; }
	};
};