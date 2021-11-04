// last revising was at 04.11.21

#pragma once 

namespace D3D11Framework
{
	class InputManager;

	struct DescWindow
	{
		DescWindow() : 
			caption(L""),
			posx(200),
			posy(20),
			width(640),
			height(480),
			resizing(true)
		{}

		std::wstring caption;
		int posx;
		int posy;
		int width;
		int height;
		bool resizing;
	};

	class Window
	{
	public:
		Window(void);

		static Window* Get() { return m_wndthis; }

		// creation and initialization of the window
		bool Create(const DescWindow &desc);

		// handling of event messages from the window
		void RunEvent(void);

		// closing of the window and releasing the memory
		void Close(void);

		void SetInputManager(InputManager* inputManager);

		HWND Window::GetHWND(void) const { return m_hwnd; }
		int Window::GetLeft(void) const { return m_desc.posx; }
		int Window::GetTop(void) const { return m_desc.posy; }
		int Window::GetWidth(void) const { return m_desc.width; }
		int Window::GetHeight(void) const { return m_desc.height; }

		const std::wstring& GetCaption(void) const { return m_desc.caption; }

		// if there is an exit, then notifies handlers about it
		bool IsExit(void) const { return m_isexit; }

		// if the window is active, then notifies handlers about it
		bool IsActive(void) const { return m_active; }

		bool IsResize(void)
		{
			bool ret = m_isresize;
			m_isresize = false;
			return ret;
		}

		LRESULT WndProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam);
	private:
		void m_UpdateWindowState(void);

		static Window* m_wndthis;

		HWND m_hwnd;
		DescWindow m_desc;
		InputManager* m_inputManager;

		bool m_isexit;
		bool m_active;
		bool m_maximized;
		bool m_minimized;
		bool m_isresize;
	};

	LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}





































/*
#pragma once

namespace D3D11Framework
{
//-------------------------------------------------------------------

	class InputManager;

	// the window description struct
	struct DescWindow
	{
		DescWindow() :
			caption(L""),			// a heading of the window
			width(640),
			height(480),
			posx(200),
			posy(20),
			resizing(true)			// can we resize the window?
		{}

		int posx;
		int posy;
		std::wstring caption;		// the window caption
		int width;					// the width of client part of the window
		int height;					// the height of client part of the window
		bool resizing;

	};


	class Window
	{
	public:
		Window();

		static Window* Get() { return m_wndthis; }

		// create a window
		bool Create(const DescWindow &desc);

		// window events handling
		void RunEvent(void);

		// close the window
		void Close(void);

		void SetInputManager(InputManager* inputManager);

		HWND Window::GetHWND() const { return m_hwnd; }
		int Window::GetLeft() const { return m_desc.posx; }
		int Window::GetTop() const { return m_desc.posy; }
		int Window::GetWidth() const { return m_desc.width; }
		int Window::GetHeight() const { return m_desc.height; }

		// return the window caption
		const std::wstring& GetCaption() const { return m_desc.caption; }

		// notifies if there was a message about exit
		bool IsExit() const { return m_isexit; }
		// notifies about window activity
		bool IsActive() const { return m_active; }
		// notifies about window changes
		// warning: after calling it notifies the window about event handling
		bool IsResize()
		{
			bool ret = m_isresize;
			m_isresize = false;
			return ret;
		}

		// events handling
		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void m_UpdateWindowState();

		static Window* m_wndthis;

		DescWindow m_desc;		// the window description
		InputManager* m_inputManager;
		HWND m_hwnd;			// the window handler
		bool m_isexit;			// the flag which notifies about the exit event
		bool m_active;			// is the window active?
		bool m_minimized;
		bool m_maximized;
		bool m_isresize;		// if the window changes its size
	};



	// events handling
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------
}

*/
