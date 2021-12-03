// the last revising was in 03.12.21

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
	class Framework
	{
	public:
		Framework();
		~Framework();

		bool Init(void);
		void Run(void);
		void Close(void);

		void AddInputListener(InputListener* listener);
		void SetRender(Render* render);

	private:
		bool m_frame();			// handling of each frame

		Window* m_wnd;
		Render* m_render;
		InputManager* m_input;
		bool m_init;			// is set to true if the framework was initialized
	};
}