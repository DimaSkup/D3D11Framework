// last revising at 01.11.21

#include "stdafx.h"
#include "Log.h"

#define LOGNAME "log.txt"

namespace D3D11Framework
{
	Log* Log::m_instance = nullptr;

	Log::Log(void)
	{
		if (!m_instance)
		{
			m_instance = this;
			m_file = nullptr;
			m_init();
		}
		else
		{
			Err("Log::Log(): the Log instance has already created");
		}
	}

	Log::~Log(void)
	{
		m_close();
		m_instance = nullptr;
	}

	void Log::m_init(void)
	{	
		if (fopen_s(&m_file, LOGNAME, "w") == 0)
		{
			char timer[9];
			char date[9];

			_strtime_s(timer, 9);
			_strdate_s(date, 9);

			fprintf(m_file, "The beginning of the log file: %s %s\n", date, timer);
			fprintf(m_file, "------------------------------------\n\n");
		}
		else
		{
			printf("Log::m_init(): an error occured during creation of the log file\n");
			m_file = nullptr;
		}
	}

	void Log::m_close(void)
	{
		if (!m_file)
			return;

		char timer[9];
		char date[9];

		_strtime_s(timer, 9);
		_strdate_s(date, 9);

		fprintf(m_file, "---------------------------------------\n");
		fprintf(m_file, "The end of the log file: %s %s\n", date, timer);

		fflush(m_file);
		fclose(m_file);
	}

	void Log::Print(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("", buffer);

			delete[] buffer;
			buffer = nullptr;
		}

		va_end(args);
	}

	void Log::Debug(const char* message, ...)
	{
#ifdef _DEBUG
		
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("*DEBUG: ", buffer);

			delete[] buffer;
			buffer = nullptr;
		}

		va_end(args);

#endif
	}

	void Log::Err(const char* message, ...)
	{
		va_list args;
		int len = 0;
		char* buffer = nullptr;

		va_start(args, message);
		len = _vscprintf(message, args) + 1;

		buffer = new (std::nothrow) char[len];

		if (buffer)
		{
			vsprintf_s(buffer, len, message, args);
			m_print("*ERROR: ", buffer);

			delete[] buffer;
			buffer = nullptr;
		}

		va_end(args);
	}

	void Log::m_print(const char* levtext, const char* text)
	{
		clock_t clockTime = clock();
		char timer[9];

		_strtime_s(timer, 9);

		printf("%s::%I64d: %s%s\n", timer, clockTime, levtext, text);

		if (m_file)
		{
			fprintf(m_file, "%s::%I64d: %s%s\n", timer, clockTime, levtext, text);
			fflush(m_file);
		}
	}
}






















