#pragma once

namespace D3D11Framework
{
	class Render
	{
	public:
		Render(void);
		virtual ~Render(void);

		virtual bool Init(HWND hWnd) = 0;
		virtual bool Draw(void) = 0;
		virtual bool Close(void) = 0;
	};
}