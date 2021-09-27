#pragma once

namespace D3D11Framework
{
//---------------------------------------------

	class Log
	{
	public:
		Log(void);
		~Log(void);

		static Log* Get(void) { return m_instance; }

		void Print(const char* format, ...);
		void Debug(const char* format, ...);
		void Err(const char* format, ...);

	private:
		static Log* m_instance;

		void m_init(void);
		void m_close(void);
		void m_print(const char* levtext, const char* text);

		FILE* m_file;
	};

//---------------------------------------------
}