// last revising at 11.11.21

#pragma once

#include "InputCodes.h"

namespace D3D11Framework
{
//------------------------------------------------------------------------

	class InputListener;

	class InputManager
	{
	public:
		void Init(void);
		void Close(void);

		// get a message about some event from Windows, 
		// handle this message and notify the listeners
		void Run(const UINT &msg, WPARAM wParam, LPARAM lParam);

		// add a new listener
		void AddListener(InputListener* Listener);

		// set the window zone
		void SetWinRect(const RECT &winrect);

	private:
		// create a mouse moving event
		void m_eventcursor(void);
		// create a mouse clicking event
		void m_eventmouse(const eMouseKeyCodes kc, bool press);
		// create a mouse wheel rotating event
		void m_mousewheel(short Value);
		// create a keybutton event
		void m_eventkey(const eKeyCodes kc, const wchar_t wc, bool press);

		std::list<InputListener*> m_Listener;	// the list of our listeners

		RECT m_windowrect;
		int m_curx;
		int m_cury;
		int m_MouseWheel;
	};

//------------------------------------------------------------------------
}
