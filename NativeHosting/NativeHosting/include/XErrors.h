#pragma once

#include <stdio.h>
#include <errno.h>

#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#else
#endif

namespace raf_tools
{
	void Trace(const std::string& content, bool crlf = true)
	{
#if defined(WIN32) || defined(WIN64)
		OutputDebugStringA(content.c_str());
		if (crlf)
			OutputDebugStringA("\r\n");
#else
		std::clog << content.c_str();
		if(crlf)
			std::clog << std::endl;

		std::flush(std::clog);
#endif
	}


	std::string GetLastError()
	{
#if defined(WIN32) || defined(WIN64)
		std::string message;
		LPSTR pMsg;
		auto lastError = ::GetLastError();
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsg, 0, NULL);
		message.append(pMsg);
		LocalFree((LPVOID)pMsg);

		return message;
#else
		std::string err(strerror(errno));
		return err;
#endif
	}

	//// format the last error according to the specified locale
	//// Examples of locales are: "en_US.UTF-8", "de_DE", "ja_JP"
	//std::string GetLastError(const std::string& locale)
	//{
	//	auto current = std::locale("");
	//	std::setlocale(LC_ALL, locale.c_str());
	//	std::string err(strerror(errno));
	//	std::setlocale(LC_ALL, current.name().c_str());
	//	return err;
	//}


}