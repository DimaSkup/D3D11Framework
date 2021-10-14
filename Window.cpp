#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
	Window* Window::m_wndthis = nullptr;

	Window::Window(void) :
		m_inputManager(nullptr),
		m_hwnd(NULL),
		m_isexit(false),
		m_active(true),
		m_maximized(false),
		m_minimized(false),
		m_isresize(false)
	{
		if (!m_wndthis)
			m_wndthis = nullptr;
		else
			Log::Get()->Err("The window has already been created");
	}

	bool Window::Create(const DescWindow &desc)
	{
		Log::Get()->Debug("Window::Create()");

		m_desc = desc;

		WNDCLASSEXW wc;
		//ZeroMemory(&wc, sizeof(WNDCLASSEXW));

		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = StaticWndProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hInstance = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"D3D11F";

		if (!RegisterClassEx(&wc))
		{
			Log::Get()->Err("Couldn't register the window class");

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
			Log::Get()->Err("Couldn't create the window");

			return false;
		}

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		return true;
	}

	void Window::RunEvent(void)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Close(void)
	{
		if (m_hwnd)
			DestroyWindow(m_hwnd);
		m_hwnd = nullptr;

		Log::Get()->Debug("Window::Close()");
	}

	LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			m_isexit = true;
			return 0;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE)
				m_active = true;
			else
				m_active = false;
			return 0;
		case WM_MOVE:
			m_desc.posx = LOWORD(lParam);
			m_desc.posy = HIWORD(lParam);
			
		}
	}
}