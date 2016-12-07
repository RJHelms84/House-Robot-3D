#pragma once

namespace Batman
{
	class IEvent //event interface
	{
	protected:
		int id;	//unique id for event
	public:
		IEvent();
		virtual ~IEvent() {}
		int getID() {return id;}
	};

	enum eventtype
	{
		EVENT_TIMER = 10,
		EVENT_KEYPRESS = 20,
		EVENT_KEYRELEASE = 30,
		EVENT_MOUSECLICK = 40,
		EVENT_MOUSEMOTION = 50,
		EVENT_MOUSEWHEEL = 60,
		EVENT_MOUSEMOVE = 70,
		EVENT_XBUTTON = 80,
		EVENT_ENTITYUPDATE = 90,
		EVENT_ENTITYRENDER = 100
	};

	//all events implement the interface
	//below classes are derived from IEvent class

	class KeyPressEvent : public IEvent //key press
	{
	public:
		int keycode;					//DirectInput code
		KeyPressEvent(int key);			//raise event with code
	};

	class KeyReleaseEvent : public IEvent
	{
	public:
		int keycode;					//DirectInput code
		KeyReleaseEvent(int key);			//raise event with code
	};

	class MouseClickEvent : public IEvent
	{
	public:
		int button;					//DirectInput code
		MouseClickEvent(int btn);			//raise event with code
	};

	class MouseMotionEvent : public IEvent
	{
	public:
		int deltax, deltay;
		MouseMotionEvent(int dx, int dy);
	};

	class MouseWheelEvent : public IEvent
	{
	public:
		int wheel;
		MouseWheelEvent(int whl);
	};

	class MouseMoveEvent : public IEvent
	{
	public:
		int posx, posy;
		MouseMoveEvent(int px, int py);
	};

	class XButtonEvent : public IEvent
	{
	public:
		XINPUT_STATE PadState;
		XboxPad* padPointer;
		XButtonEvent(XINPUT_STATE padState, XboxPad* padPtr);
	};

	class EntityRenderEvent : public IEvent
	{
	public:
		Batman::Entity* entity;
		EntityRenderEvent(Entity* e);
	};

	class EntityUpdateEvent : public IEvent
	{
	public:
		Batman::Entity* entity;
		EntityUpdateEvent(Entity* e);
	};
};