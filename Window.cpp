#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
//---------------------------------------------------------

	Window *Window::m_wndthis = nullptr;

	Window::Window(void) :
		m_inputManager(nullptr),
		m_hwnd(0),
		m_isexit(false),
		m_active(true),
		m_minimized(false),
		m_maximized(false),
		m_isresize(false)
	{
		if (!m_wndthis)
			m_wndthis = this;
		else
			Log::Get()->Err("The Window has already been created");
	}

	bool Window::Create(const DescWindow &desc)
	{
		Log::Get()->Debug("Window Create");
		m_desc = desc;

		WNDCLASSEXW wnd;
		ZeroMemory(&wnd, sizeof(WNDCLASSEX));

		wnd.cbSize = sizeof(WNDCLASSEXW);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = StaticWndProc;
		wnd.cbClsExtra = 0;
		wnd.cbWndExtra = 0;
		wnd.hInstance = 0;
		wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wnd.hIconSm = wnd.hIcon;
		wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wnd.lpszMenuName = NULL;
		wnd.lpszClassName = L"D3D11F";

		if (!RegisterClassEx(&wnd))
		{
			Log::Get()->Err("Couldn't manage to register the window");
			return false;
		}

		RECT rect = { 0, 0, m_desc.width, m_desc.height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);


		m_hwnd = CreateWindowEx(NULL,
								L"D3D11F",
								m_desc.caption.c_str,
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								static_cast<long>(m_desc.posx),
								static_cast<long>(m_desc.posy),
								rect.right - rect.left,
								rect.bottom - rect.top,
								NULL,
								NULL,
								NULL,
								NULL);

		if (!m_hwnd)
		{
			Log::Get()->Err("Couldn't manage to create the window");
			return false;
		}

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		return true;
	}

//---------------------------------------------------------
}