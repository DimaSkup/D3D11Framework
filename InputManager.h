#pragma once

#include "InputCodes.h"

namespace D3D11Framework
{
	class InputListener;

	class InputManager
	{
	public:
		void Init(void);
		void Close(void);

		// get an event from Windows, do some action and notify the listeners
		void Run(const UINT &msg, WPARAM wParam, LPARAM lParam);

		// add a new listener
		void AddListener(InputListener* Listener);

		// set the window zone
		void SetWinRect(const RECT &winrect);

	private:
		// mouse moving event
		void m_eventcursor();
		// mouse clicking event
		void m_eventmouse(const eMouseKeyCodes kc, bool press);
		// mouse wheel rotating event
		void m_mousewheel(short Value);
		// keybutton event
		void m_eventkey(const eKeyCodes kc, const wchar_t wc, bool press);

		std::list<InputListener*> m_Listener;	// the list of our listeners

		RECT m_windowrect;
		int m_curx;
		int m_cury;
		int m_MouseWheel;
	};

//-----------------------------------------------------
}