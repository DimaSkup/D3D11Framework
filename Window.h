#pragma once

namespace D3D11Framework
{
//----------------------------------------------------

	class InputManager;

	struct DescWindow
	{
		DescWindow() :
			caption(L""),
			width(640),
			height(480),
			posx(200),
			posy(20),
			resizing(true)
		{}

		int posx;
		int posy;
		std::string caption;		// the window caption
		int width;					// the width of client part of the window
		int height;					// the height of client part of the window
		bool resizing;

	};


	class Window
	{
	public:
		Window();

		static Window* Get() { return m_wndthis; }

		// Create a window
		bool Create(const DescWindow &desc);

		// Window events handling
		void RunEvent();

		// Close the window
		void Close(void);

		void SetInputManager(InputManager* inputManager);

		HWND GetHWND() const { return m_hwnd; }
		int Window::GetLeft() const { return m_desc.posx }
		int Window::GetTop() const { return m_desc.posy; }
		int Window::GetWidth() const { return m_desc.width; }
		int Window::GetHeight() const { return m_desc.height; }

		// return the window caption
		const std::wstring& GetCaption() const { return m_desc.caption; }

		// notify, if there was a message about exit
		bool IsExit() const { return m_isexit; }
		// notify about window activity
		bool IsActive() const { return m_active; }
		// notify about changes of the window
		// warning: after calling notifies the window about event handling
		bool IsResize()
		{
			bool ret = m_isresize;
			m_isresize = false;
			return ret;
		}

		LRESULT WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	private:
		void m_UpdateWindowState();

		static Window* m_wndthis;

		DescWindow m_desc;		// the window description
		InputManager* m_inputManager;
		HWND m_hwnd;			// the window handler
		bool m_isexit;			// flag which notifies about an exit event
		bool m_active;			// is the window active?
		bool m_minimized;
		bool m_maximized;
		bool m_isresize;		// if the window change size
	};


	// event handler
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------
}