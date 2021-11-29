// the last revising was in 11.11.21

#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"

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
		Log::Get()->Print("Framework::Framework(): calling of the constructor");
	}

	Framework::~Framework(void)
	{
		Log::Get()->Debug("Framework::Framework(): calling of the destructor");
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
		Log::Get()->Debug("Framework::Init()");

		m_wnd = new (std::nothrow) Window();
		m_input = new (std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			Log::Get()->Err("Framework::Init(): can't allocate the memory");
			return false;
		}

		m_input->Init();

		// Set up the default values
		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			Log::Get()->Err("Framework::Init(): can't create the window");
			return false;
		}

		m_wnd->SetInputManager(m_input);

		if (!m_render->CreateDevice(m_wnd->GetHWND()))
		{
			Log::Get()->Err("Framework::Init(): can't initialize the render");
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

		m_render->Shutdown();
		_DELETE(m_render);
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

		m_render->BeginFrame();
		if (!m_render->Draw())
		{
			return false;
		}
		m_render->EndFrame();

		return true;
	}

//-------------------------------------------------------------------
}