#include "Engine.h"

namespace Batman
{
	XboxPad::XboxPad(int playerNumber)
	{
		//set the controller number
		_controllerNum = playerNumber - 1;
	}

	XINPUT_STATE XboxPad::GetState()
	{
		//Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		//Get the state
		XInputGetState(_controllerNum, &_controllerState);

		return _controllerState;
	}

	bool XboxPad::IsConnected()
	{
		//Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		//Get the state
		DWORD Result = XInputGetState(_controllerNum, &_controllerState);

		if(Result==ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void XboxPad::Vibrate(int leftVal, int rightVal)
	{
		//Create a vibration state
		XINPUT_VIBRATION Vibration;

		//zeroise the vibration
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		//set the vibration values
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		//vibrate the controller
		XInputSetState(_controllerNum, &Vibration);
	}
};