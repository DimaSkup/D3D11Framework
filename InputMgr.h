#pragma once

#include "InputCodes.h"

namespace D3D11Framework
{
//-----------------------------------------------------

	class InputListener;

	class InputMgr
	{
	public:
		void Init();
		void Close();

		void Run(const UINT &msg, WPARAM wParam, LPARAM lParam);

		void AddListener(InputListener* Listener);

		// the window zode
		void SetWinRect(const RECT &winrect);

	private:
		// mouse moving event
		void m_eventcursor();
		// mouse button event
		void m_eventmouse(const eMouseKeyCodes KeyCode, bool press);
		// mouse wheel event
		void m_mousewheel(short Value);
		// handling of the keyboard button event
		void m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press);

		std::list<InputListener*> m_Listener;


		RECT m_windowrect;
		int m_curx;
		int m_cury;
		int m_MouseWheel;
	};

//-----------------------------------------------------
}