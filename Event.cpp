#include "Engine.h"

namespace Batman
{
	//base event class constructor - resets id
	IEvent::IEvent()
	{
		id = 0;
	}

	KeyPressEvent::KeyPressEvent(int key)
	{
		id = EVENT_KEYPRESS;	//set enum
		keycode = key;			//set key
	}

	KeyReleaseEvent::KeyReleaseEvent(int key)
	{
		id = EVENT_KEYRELEASE;	//set enum
		keycode = key;			
	}

	MouseClickEvent::MouseClickEvent(int btn)
	{
		id = EVENT_MOUSECLICK;
		button = btn;
	}

	MouseMotionEvent::MouseMotionEvent(int dx, int dy)
	{
		id = EVENT_MOUSEMOTION;
		deltax = dx;
		deltay = dy;
	}

	MouseWheelEvent::MouseWheelEvent(int whl)
	{
		id = EVENT_MOUSEWHEEL;
		wheel = whl;
	}

	MouseMoveEvent::MouseMoveEvent(int px, int py)
	{
		id = EVENT_MOUSEMOVE;
		posx = px;
		posy = py;
	}

	XButtonEvent::XButtonEvent(XINPUT_STATE padState, XboxPad* padPtr)
	{
		id = EVENT_XBUTTON;
		PadState = padState;
		padPointer = padPtr;
	}

	EntityRenderEvent::EntityRenderEvent(Entity* e)
	{
		id = EVENT_ENTITYRENDER;
		entity = e;
	}

	EntityUpdateEvent::EntityUpdateEvent(Entity* e)
	{
		id = EVENT_ENTITYUPDATE;
		entity = e;
	}
};