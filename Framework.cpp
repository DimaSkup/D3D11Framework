#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Framework::Framework() :
		m_wnd(nullptr),
		m_render(nullptr),
		m_input(nullptr),
		m_init(false)
	{

	}

	Framework::~Framework()
	{

	}

	void Framework::AddInputListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	void Framework::Run()
	{
		if (m_init)
			while (m_frame());
	}

	bool Framework::Init()
	{
		m_wnd = new Window();
		m_input = new InputManager();

		if (!m_wnd || !m_input)
		{
			Log::Get()->Err("Framework::Init(): Can't allocate the memory");
			return false;
		}

		m_input->Init();

		// Set up parameters by default. 
		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			Log::Get()->Err("Framework::Init(): Can't create the window");
			return false;
		}

		m_wnd->SetInputManager(m_input);

		if (!m_render->Init(m_wnd->GetHWND()))
		{
			Log::Get()->Err("Framework::Init(): Can't create the render");
			return false;
		}

		m_init = true;
		return true;
	}

	bool Framework::m_frame()
	{
		// handle window events
		m_wnd->RunEvent();
		// if the window is inactive - finish the frame
		if (!m_wnd->IsActive())
			return true;

		// if the window has been closed - finish the engine work
		if (m_wnd->IsExit())
			return false;

		// if the window changed its size
		if (m_wnd->IsResize())
		{

		}

		if (!m_render->Draw())
			return false;

		return true;
	}

	void Framework::Close()
	{
		m_init = false;
		_CLOSE(m_render);
		_CLOSE(m_wnd);
		_CLOSE(m_input);
	}

//-------------------------------------------------------------------
}