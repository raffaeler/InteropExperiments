#pragma once

#include <string>
#include <sstream>
#include "XErrors.h"

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace raf_tools
{
	class LibraryLoader
	{
		LibraryLoader(const LibraryLoader&) = delete;
		const LibraryLoader& operator=(const LibraryLoader&) = delete;

		bool _neverUnload;
#if defined(WIN32) || defined(WIN64)
		HMODULE _hModule;
#else
		void * _hModule;
#endif

	public:
		//operator const HMODULE() const { return _hModule; }
		void* GetRawHandle() { return (void *)_hModule; }

		explicit LibraryLoader(const std::string& libraryFileName, bool neverUnload = false)
		{
			_neverUnload = neverUnload;

#if defined(WIN32) || defined(WIN64)
			_hModule = LoadLibraryA(libraryFileName.c_str());
#else
			_hModule = dlopen(libraryFileName.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
			if (_hModule == nullptr)
			{
				std::stringstream ss;
				ss << "Cannot load the specified library: ";
				ss << libraryFileName;

#if defined(WIN32) || defined(WIN64)
#else
				ss << " - " << dlerror();
#endif
				ss << std::endl;

				throw std::runtime_error(ss.str().c_str());
			}
		}

		template<typename T>
		T GetFuncAddress(const std::string& funcName)
		{
#if defined(WIN32) || defined(WIN64)
			return (T)::GetProcAddress(_hModule, funcName.c_str());
#else
			return (T)dlsym(_hModule, funcName.c_str());
#endif
		}

		~LibraryLoader()
		{
			if (_neverUnload)
			{
				return;
			}

#if defined(WIN32) || defined(WIN64)
			const auto result = FreeLibrary(_hModule);
			_ASSERTE(result);
#else
			const auto result = dlclose(_hModule);
			//_ASSERTE(result == 0);
#endif
			static_cast<void>(result);
		}

	};
}