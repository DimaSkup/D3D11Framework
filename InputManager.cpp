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
		Log::Get()->Debug("Input Manager init");
	}

	void InputManager::Close()
	{
		if (!m_Listener.empty())
			m_Listener.clear();

		Log::Get()->Debug("Input Manager close");
	}

	// set the window zone
	void InputManager::SetWinRect(const RECT &winrect)
	{
		m_windowrect.top = winrect.top;
		m_windowrect.bottom = winrect.bottom;
		m_windowrect.right = winrect.right;
		m_windowrect.left = winrect.left;
	}

	// add a new listener
	void InputManager::AddListener(InputListener* Listener)
	{
		m_Listener.push_back(Listener);
	}

	// get an event from Windows, do some action and notify the listeners
	void InputManager::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
	{
		if (m_Listener.empty())
			return;

		eKeyCodes KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];

		m_eventcursor();	// mouse moving event

		switch(msg)
		{
		case WM_KEYDOWN:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam,		// the code of virtual key
				HIWORD(lParam) & 0xFF,	// scan-code of the key
				lpKeyState,	// a pointer to the 256-byte array which contains a keyboard current state
				buffer,		// put here translated symbol or symbols Unicode
				1,			// the buffer size
				0);			// flags: 0 -- the menu is active
			m_eventkey(KeyIndex, buffer[0], true);
			break;

		case WM_KEYUP:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam,		// the code of virtual key
					  HIWORD(lParam) & 0xFF,	// scan-code of the key
					  lpKeyState,	// a pointer to the 256-byte array which contains a keyboard current state
					  buffer,		// put here translated symbol or symbols Unicode
					  1,			// the buffer size
					  0);			// flags: 0 -- the menu is active
			m_eventkey(KeyIndex, buffer[0], false);
			break;

		// LMB
		case WM_LBUTTONDOWN:
			m_eventmouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			m_eventmouse(MOUSE_LEFT, false);
			break;

		// RMB
		case WM_RBUTTONDOWN:
			m_eventmouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			m_eventmouse(MOUSE_RIGHT, false);
			break;

		// MMB
		case WM_MBUTTONDOWN:
			m_eventmouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			m_eventmouse(MOUSE_MIDDLE, false);


		// MOUSE WHEEL
		case WM_MOUSEWHEEL:
			m_mousewheel((short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
	}

	void InputManager::m_eventcursor()
	{
		POINT Position;
		GetCursorPos(&Position);		// get current position of the cursor

		Position.x -= m_windowrect.left;
		Position.y -= m_windowrect.top;

		if (m_curx == Position.x && m_cury == Position.y)
			return;

		m_curx = Position.x;
		m_cury = Position.y;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseMove(MouseEvent(m_curx, m_cury)) == true)
				return;
		}
	}

	void InputManager::m_eventmouse(const eMouseKeyCodes Code, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;

			// button is pressed
			if (press == true)
			{
				if ((*it)->MousePressed(MouseEventClick(Code, m_curx, m_cury)) == true)
					return;
			}
			// button is released
			else
			{
				if ((*it)->MouseReleased(MouseEventClick(Code, m_curx, m_cury)) == true)
					return;
			}
		}
	}

	void InputManager::m_mousewheel(short Value)
	{
		if (m_MouseWheel == Value)
			return;

		m_MouseWheel = Value;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseWheel(MouseEventWheel(m_MouseWheel, m_curx, m_cury)) == true)
				return;
		}
	}

	void InputManager::m_eventkey(const eKeyCodes KeyCode, const wchar_t ch, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;

			// button is pressed
			if (press == true)
			{
				if ((*it)->KeyPressed(KeyEvent(ch, KeyCode)) == true)
					return;
			}
			// button is released
			else
			{
				if ((*it)->KeyReleased(KeyEvent(ch, KeyCode)) == true)
				{
					return;
				}
			}
		}
	}

//---------------------------------------------------------
}