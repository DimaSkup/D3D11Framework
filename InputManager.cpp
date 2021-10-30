// last revising at 30.10.21

#include "stdafx.h"
#include "InputCodes.h"
#include "InputListener.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
	void InputManager::Init()
	{
		m_MouseWheel = m_curx = m_cury = 0;
		Log::Get()->Debug("Input Manager init()");
	}

	void InputManager::Close()
	{
		if (!m_Listener.empty())
			m_Listener.clear();

		Log::Get()->Debug("Input Manager close()");
	}

	void InputManager::SetWinRect(const RECT &windowrect)
	{
		m_windowrect.top = windowrect.top;
		m_windowrect.bottom = windowrect.bottom;
		m_windowrect.right = windowrect.right;
		m_windowrect.left = windowrect.left;
	}

	void InputManager::AddListener(InputListener* Listener)
	{
		m_Listener.push_back(Listener);
	}


	void InputManager::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
	{
		if (m_Listener.empty())
		{
			Log::Get()->Err("InputManager:Run(): There is no listener yet");
			return;
		}

		eKeyCodes KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];

		m_eventcursor();

		switch (msg)
		{
		// KEYBUTTON events
		case WM_KEYDOWN:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam,			// the code of virtual key
				HIWORD(lParam) & 0xFF,	// scan-code of the key
				lpKeyState,				// a pointer to the 256-byte array which contains a keyboard current state
				buffer,					// put here translated symbol or symbols Unicode
				1,						// the buffer size
				0);						// flags: 0 -- the menu is active
			m_eventkey(KeyIndex, buffer[0], true);
			break;

		case WM_KEYUP:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam,			// the code of virtual key
				HIWORD(lParam) & 0xFF,	// scan-code of the key
				lpKeyState,				// a pointer to the 256-byte array which contains a keyboard current state
				buffer,					// put here translated symbol or symbols Unicode
				1,						// the buffer size
				0);						// flags: 0 -- the menu is active
			m_eventkey(KeyIndex, buffer[0], false);
			break;


		// MOUSE LEFT BUTTON events
		case WM_LBUTTONDOWN:
			m_eventmouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			m_eventmouse(MOUSE_LEFT, false);
			break;

		// MOUSE RIGHT BUTTON events
		case WM_RBUTTONDOWN:
			m_eventmouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			m_eventmouse(MOUSE_RIGHT, false);
			break;

		// an event of MOUSE WHEEL clicking
		case WM_MBUTTONDOWN:
			m_eventmouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			m_eventmouse(MOUSE_MIDDLE, false);
			break;

		// an event of MOUSE WHEEL rotating
		case WM_MOUSEWHEEL:
			m_mousewheel(static_cast<short>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA));
			break;
		}
	}


	void InputManager::m_eventcursor()
	{
		POINT pos;
		GetCursorPos(&pos);

		pos.x -= m_windowrect.left;
		pos.y -= m_windowrect.bottom;

		if (m_curx == pos.x && m_cury == pos.y)
			return;

		m_curx = pos.x;
		m_cury = pos.y;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// handle this event
			else if ((*it)->MouseMove(MouseEvent(m_curx, m_cury)) == true)	
				return;
		}
	}

	// create an event of mouse button clicking/releasing and pass it to the handler
	void InputManager::m_eventmouse(const eMouseKeyCodes code, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;

			// mouse button is pressed
			if (press == true)
			{
				// handle this event
				if ((*it)->MousePressed(MouseEventClick(code, m_curx, m_cury)) == true)
					return;
			}
			// mouse button is released
			{
				// handle this event
				if ((*it)->MouseReleased(MouseEventClick(code, m_curx, m_cury)) == true)
					return;
			}
		}
	}

	// create an event of mouse wheel rotating
	void InputManager::m_mousewheel(short Value)
	{
		if (m_MouseWheel == Value)
			return;

		m_MouseWheel = Value;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// handle this event
			else if ((*it)->MouseWheel(MouseEventWheel(m_MouseWheel, m_curx, m_cury)) == true)
				return;
		}
	}

	// create an event of keybutton clicking/releasing
	void InputManager::m_eventkey(const eKeyCodes kc, const wchar_t wc, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;

			// keybutton is pressed
			if (press == true)
			{
				// handle this event
				if ((*it)->KeyPressed(KeyEvent(kc, wc)) == true)
					return;
			}
			// keybutton is released
			else
			{
				// handle this event
				if ((*it)->KeyReleased(KeyEvent(kc, wc)) == true)
					return;
			}
		}
	}

}