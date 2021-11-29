// the last revising was in 11.11.21

#pragma once

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	class Framework
	{
	public:
		Framework(void);
		~Framework(void);

		bool Init(void);
		void Run(void);
		void Close(void);

		void SetRender(Render* render);
		void AddInputListener(InputListener* inputListener);

	protected:
		bool m_frame();		// handling of each the frame

		Window* m_wnd;
		Render* m_render;
		InputManager* m_input;
		Log m_log;
		bool m_init;		// if it has been initialized
	};

//-------------------------------------------------------------------
}