// the last revising was at 04.11.21

#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
	Window* Window::m_wndthis = nullptr;

	Window::Window(void) :
		m_inputManager(nullptr),
		m_hwnd(0),
		m_isexit(false),
		m_isresize(false),
		m_active(true),
		m_maximized(false),
		m_minimized(false)
	{
		if (!m_wndthis)
			m_wndthis = this;
		else
			Log::Get()->Err("Can't create a Window class instance");
	}

	bool Window::Create(const DescWindow &desc)
	{
		Log::Get()->Debug("Window::Create()");

		m_desc = desc;

		WNDCLASSEX wcd;
		//ZeroMemory(&wcd, sizeof(WNDCLASSEXW));

		wcd.cbSize = sizeof(WNDCLASSEXW);
		wcd.style = CS_HREDRAW | CS_VREDRAW;
		wcd.lpfnWndProc = StaticWndProc;
		wcd.hInstance = 0;
		wcd.cbClsExtra = 0;
		wcd.cbWndExtra = 0;
		wcd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wcd.hIconSm = wcd.hIcon;
		wcd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcd.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcd.lpszMenuName = NULL;
		wcd.lpszClassName = L"D3D11F";

		if (!RegisterClassEx(&wcd))
		{
			Log::Get()->Err("Didn't manage to register the window");
			return false;
		}


		RECT rect = { 0, 0, m_desc.width, m_desc.height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		long lwidth = rect.right - rect.left;
		long lheight = rect.bottom - rect.top;

		long lleft = (long)m_desc.posx;
		long ltop = (long)m_desc.posy;

		m_hwnd = CreateWindowEx(NULL, 
								L"D3D11F",
								m_desc.caption.c_str(),
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			lleft,
			ltop,
			lwidth,
			lheight,
								NULL,
								NULL,
								NULL,
								NULL);

		if (!m_hwnd)
		{
			Log::Get()->Err("Didn't manage to create the window");
			return false;
		}

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		Log::Get()->Debug("Window::Create(): the window created successfully");

		return true;
	}

	void Window::RunEvent(void)
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Close(void)
	{
		if (m_hwnd)
			DestroyWindow(m_hwnd);
		m_wndthis = nullptr;

		Log::Get()->Debug("Window::Close()");
	}

	LRESULT Window::WndProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam)
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
			m_UpdateWindowState();

			return 0;
		case WM_SIZE:
			if (!m_desc.resizing)
				return 0;

			m_desc.width = LOWORD(lParam);
			m_desc.height = HIWORD(lParam);
			m_isresize = true;

			if (wParam == SIZE_MAXIMIZED)
			{
				m_active = true;
				m_minimized = false;
				m_maximized = true;
			}
			else if (wParam == SIZE_MINIMIZED)
			{
				m_active = false;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_minimized)
				{
					m_active = true;
					m_minimized = false;
				}
				else if (m_maximized)
				{
					m_active = true;
					m_maximized = false;
				}
			}

			m_UpdateWindowState();

			return 0;

		case WM_MOUSEMOVE: 
		case WM_KEYDOWN: case WM_KEYUP:
		case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
				if (m_inputManager)
					m_inputManager->Run(message, wParam, lParam);
				return 0;
		}

		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	void Window::SetInputManager(InputManager* inputManager)
	{
		Log::Get()->Debug("Window::SetInputManager()");
		m_inputManager = inputManager;
		m_UpdateWindowState();
	}

	void Window::m_UpdateWindowState()
	{
		RECT ClientRect;
		ClientRect.left = m_desc.posx;
		ClientRect.top = m_desc.posy;
		ClientRect.right = m_desc.width;
		ClientRect.bottom = m_desc.height;
		
		if (m_inputManager)
			m_inputManager->SetWinRect(ClientRect);
	}

	LRESULT CALLBACK D3D11Framework::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return Window::Get()->WndProc(hWnd, message, wParam, lParam);
	}
}