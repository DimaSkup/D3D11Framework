#pragma once

namespace D3D11Framework
{
//----------------------------------------------------------------

	// mouse events
	struct MouseEvent
	{
		MouseEvent(int nx, int ny) 
			: x(nx), y(ny) {}

		// mouse coordinates
		int x;
		int y;
	};

	// mouse click event
	struct MouseEventClick : public MouseEvent
	{
		MouseEventClick(eMouseKeyCodes b, int nx, int ny)
			: MouseEvent(nx, ny), btn(b) {}

		const eMouseKeyCodes btn;		// keybutton
	};

	// mouse wheel event
	struct MouseEventWheel : public MouseEvent
	{
		MouseEventWheel(int nwheel, int nx, int ny)
			: MouseEvent(nx, ny), wheel(nwheel) {}

		int wheel;
	};

	// keybutton event
	struct KeyEvent
	{
		KeyEvent(wchar_t c, eKeyCodes kc) 
			: wc(c), code(kc) {}

		const wchar_t wc;
		const eKeyCodes code;
	};


	class InputListener
	{
	public:
		// if methods return true -- the event is no more handled

		// mouse button is pressed
		virtual bool MousePressed(const MouseEventClick &arg) { return false; }
		// mouse button is released
		virtual bool MouseReleased(const MouseEventClick &arg) { return false; }
		// wheel is used
		virtual bool MouseWheel(const MouseEventWheel &arg) { return false; }
		// mouse is moving
		virtual bool MouseMove(const MouseEvent &arg) { return false; }

		// keybutton is pressed
		virtual bool KeyPressed(const KeyEvent &arg) { return false; }
		// keybutton is released
		virtual bool KeyReleased(const KeyEvent &arg) { return false; }
	};

//----------------------------------------------------------------
}