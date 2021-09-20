#include "stdafx.h"
#include "Log.h"

#define LOGNAME "log.txt"

namespace D3D11Framework
{
//----------------------------------------------------------

	Log* Log::m_instance = nullptr;

	Log::Log()
	{
		if (!m_instance)
		{
			m_file = nullptr;
			m_instance = this;
			m_init();
		}
		else
		{
			Err("The log file has already been created");
		}
	}

	
	Log::~Log()
	{
		m_close();
		m_instance = nullptr;
	}

	void Log::m_init()
	{
		if (fopen_s(&m_file, LOGNAME, "w") == 0)
		{
			char timer[9];
			_strtime_s(timer, 9);
			char date[9];
			_strdate_s(date, 9);

			fprintf(m_file, "Log has created at: %s %s. \n", date, timer);
			fprintf(m_file, "---------------------------------------\n\n");
		}
		else
		{
			printf("Error during creation of the log file...\n");
			m_file = nullptr;
		}
	}

	void Log::m_close()
	{
		if (!m_file)
			return;

		char timer[9];
		_strtime_s(timer, 9);
		char date[9];
		_strdate_s(date, 9);

		fprintf(m_file, "\n---------------------------------------\n");
		fprintf(m_file, "The end of the log: %s %s.", date, timer);
	}

	void Log::Print(const char *message, ...)
	{
		va_list args;
		va_start(args, message);

		int len = _vscprintf(message, args)		// _vsprintf doesn't 
										+ 1;	// terminating '\0'	
		//char *buffer = static_cast<char*>(malloc(len * sizeof(char)));
		char* buffer = new char [len * sizeof(char)];
		vsprintf_s(buffer, len, message, args);	
		m_print("", buffer);
		va_end(args);

		delete[] buffer;
		buffer = nullptr;
	}

	void Log::Debug(const char *message, ...)
	{
#ifdef _DEBUG
		va_list args;
		va_start(args, message);

		int len = _vscprintf(message, args) + 1;
		char *buffer = new char [len * sizeof(char)];
		vsprintf_s(buffer, len, message, args);
		m_print("*DEBUG: ", buffer);
		va_end(args);

		delete[] buffer;
		buffer = nullptr;
#endif
	}

	void Log::Err(const char *message, ...)
	{
		va_list args;
		va_start(args, message);
		int len = _vscprintf(message, args) + 1;
		char *buffer = new char [len * sizeof(char)];
		vsprintf_s(buffer, len, message, args);
		m_print("*ERROR: ", buffer);
		va_end(args);

		delete[] buffer;
		buffer = nullptr;
	}

	void Log::m_print(const char* levtext, const char* text)
	{
		char timer[9];
		_strtime_s(timer, 9);
		clock_t cl = clock();

		printf("%s::%d: %s%s\n", timer, cl, levtext, text);

		if (m_file)
		{
			fprintf(m_file, "%s::%s: %s%s\n", timer, cl, levtext, text);
			fflush(m_file);
		}
	}

//----------------------------------------------------------
}