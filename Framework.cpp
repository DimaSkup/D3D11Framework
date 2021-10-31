// the last revising was in 31.10.21

#include "stdafx.h"
#include "Framework.h"
#include "macros.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	// ---------------------
	//   PUBLIC METHODS
	// ---------------------
	Framework::Framework(void) :
		m_wnd(nullptr),
		m_input(nullptr),
		m_render(nullptr),
		m_init(false)
	{
		m_log->Debug("Framework::Framework(): calling of the constructor");
	}

	Framework::~Framework(void)
	{
		m_log->Debug("Framework::Framework(): calling of the destructor");
	}

	void Framework::AddInputListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	void Framework::SetRender(Render* render)
	{
		m_render = render;
	}

	bool Framework::Init(void)
	{
		m_log->Debug("Framework::Init()");

		m_wnd = new (std::nothrow) Window();
		m_input = new (std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			m_log->Err("Framework::Init(): can't allocate the memory");
			return false;
		}

		m_input->Init();

		// Set up the default values
		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			m_log->Err("Framework::Init(): can't create the window");
			return false;
		}

		m_wnd->SetInputManager(m_input);

		if (m_render->Init(m_wnd->GetHWND()))
		{
			m_log->Err("Framework::Init(): can't initialize the render");
			return false;
		}

		m_init = true;
		return true;
	}

	void Framework::Run(void)
	{
		if (m_init)
			while (m_frame());
	}

	void Framework::Close(void)
	{
		m_init = false;

		_CLOSE(m_render);
		_CLOSE(m_input);
		_CLOSE(m_wnd);
	}

	

	// ---------------------
	//   PRIVATE METHODS
	// ---------------------

	bool Framework::m_frame(void)
	{
		// handle window events
		m_wnd->RunEvent();

		// if the window is inactive -- finish the frame
		if (!m_wnd->IsActive())
		{
			return true;
		}

		// if there is an exit -- finish the engine work
		if (m_wnd->IsExit())
		{
			return false;
		}

		// if the window changed its size 
		if (m_wnd->IsResize())
		{

		}

		if (!m_render->Draw())
		{
			return false;
		}

		return true;
	}

//-------------------------------------------------------------------
}