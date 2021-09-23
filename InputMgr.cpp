#include "stdafx.h"
#include "InputMgr.h"
#include "InputCodes.h"
#include "InputListener.h"
#include "Log.h"

namespace D3D11Framework
{
//---------------------------------------------------------

	void InputMgr::Init()
	{
		m_MouseWheel = m_curx = m_cury = 0;
		Log::Get()->Debug("InputMgr init");
	}

	void InputMgr::Close()
	{
		if (!m_Listener.empty())
		{
			m_Listener.clear();
		}

		Log::Get()->Debug("InputMgr close");
	}

//---------------------------------------------------------
}