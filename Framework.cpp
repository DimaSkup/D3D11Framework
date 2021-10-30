// the last revising was in 24.10.21

#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Framework::Framework(void) :
		m_wnd(nullptr),
		m_render(nullptr),
		m_input(nullptr),
		m_init(false)
	{
		m_log = Log::Get();
	}

	Framework::~Framework(void)
	{

	}

	bool Framework::Init(void)
	{
		m_wnd = new(std::nothrow) Window();
		m_input = new(std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			m_log->Err("Framework::Init(): can't allocate the memory");
			return false;
		}

		m_input->Init();

		// Set up default values for the window
		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			m_log->Err("Framework::Init(): can't create the window");
			return false;
		}

		m_wnd->SetInputManager(m_input);

		if (!m_render->Init(m_wnd->GetHWND()))
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

	void Framework::AddInputListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	bool Framework::m_frame(void)
	{
		// handle window events
		m_wnd->RunEvent();

		// if the window is inactive -- finish the frame
		if (!m_wnd->IsActive())
			return true;

		// if the window has been close -- finish the engine work
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

//-------------------------------------------------------------------
}






























/*#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Framework::Framework(void) :
		m_wnd(nullptr),
		m_render(nullptr),
		m_input(nullptr),
		m_init(nullptr)
	{

	}

	Framework::~Framework(void)
	{

	}

	void Framework::AddInputListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	void Framework::Run(void)
	{
		if (m_init)
			while (m_frame());
	}

	bool Framework::Init(void)
	{
		m_wnd = new(std::nothrow) Window();
		m_input = new(std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			Log::Get()->Err("Framework::Init(): can't allocated the memory");
			return false;
		}

		m_input->Init();

		// Set up parameters by default
		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			Log::Get()->Err("Framework::Init(): can't create the window");
			return false;
		}

		m_wnd->SetInputManager(m_input);

		if (!m_render->Init(m_wnd->GetHWND()))
		{
			Log::Get()->Err("Framework::Init(): can't initialize the render");
			return false;
		}

		m_init = true;
		return true;
	}

	bool Framework::m_frame(void)
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

	void Framework::Close(void)
	{
		m_init = false;
		_CLOSE(m_input);
		_CLOSE(m_render);
		_CLOSE(m_wnd);
	}
//-------------------------------------------------------------------
}

*/