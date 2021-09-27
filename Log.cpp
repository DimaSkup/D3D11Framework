#include "stdafx.h"
#include "Log.h"

#define LOGNAME "log.txt"

namespace D3D11Framework
{
//--------------------------------------------------

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
			Err("The log file has already been created...");
		}
	}

	Log::~Log()
	{
		m_close();
		m_instance = nullptr;
	}

	void Log::m_init()
	{
		char timer[9];
		char date[9];

		_strtime_s(timer, 9);
		_strdate_s(date, 9);

		if (fopen_s(&m_file, LOGNAME, "w") == 0)
		{
			fprintf(m_file, "%s %s: The log file has been created successfully!\n", timer, date);
			fprintf(m_file, "---------------------------------------------------\n");
		}
		else
		{
			Err("%s %s: An error has occured during creation of the log file...\n", timer, date);
			m_file = nullptr;
		}
	}

	void Log::m_close()
	{
		if (!m_file)
			return;

		char timer[9];
		char date[9];

		_strtime_s(timer, 9);
		_strdate_s(date, 9);

		fprintf(m_file, "\n---------------------------------\n");
		fprintf(m_file, "%s %s: The end of the log file.\n", timer, date);

		fflush(m_file);
		fclose(m_file);
	}

	void Log::Print(const char* format, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, format);
		len = _vscprintf(format, args) + 1;

		buffer = new char[len];
		assert(buffer);

		vsprintf_s(buffer, len, format, args);
		m_print("", buffer);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);
	}

	void Log::Debug(const char* format, ...)
	{
#ifdef _DEBUG

		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, format);
		len = _vscprintf(format, args) + 1;

		buffer = new char[len];
		assert(buffer);

		vsprintf_s(buffer, len, format, args);
		m_print("*DEBUG", buffer);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);

#endif
	}

	void Log::Err(const char* format, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, format);
		len = _vscprintf(format, args) + 1;

		buffer = new char[len];
		assert(buffer);

		vsprintf_s(buffer, len, format, args);
		m_print("*ERROR", buffer);

		delete[] buffer;
		buffer = nullptr;
		va_end(args);
	}

	void Log::m_print(const char* levtext, const char* text)
	{
		time_t cl = clock();		// time clock
		char timer[9];

		_strtime_s(timer, 9);

		printf("%s %d: %s %s.\n", timer, cl, levtext, text);

		if (m_file)
		{
			fprintf(m_file, "%s::%d: %s %s.\n", timer, cl, levtext, text);
			fflush(m_file);
		}
	}

//--------------------------------------------------
}