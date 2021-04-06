#pragma once

// -------------------------------------------------------------------------------------
// Copyright (C) Raffaele Rialdi. All right reserved.
// -------------------------------------------------------------------------------------
// The CoreCLR code and related libraries are owned by Microsoft and the .NET Foundation
// https://github.com/dotnet/coreclr/blob/master/LICENSE.TXT
// -------------------------------------------------------------------------------------

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <memory>

#include "XErrors.h"
#include "XUtilities.h"
#include "XDynamicLoad.h"
#include "coreclrhost.h"

#ifndef SUCCEEDED
#define SUCCEEDED(Status) ((Status) >= 0)
#endif // !SUCCEEDED

#define serverGcVar	"CORECLR_SERVER_GC"
#define globalizationInvariantVar "CORECLR_GLOBAL_INVARIANT"

// Errors from coreclr_initialize
// https://github.com/dotnet/coreclr/blob/master/src/mscorlib/src/System/IO/__HResults.cs
// 0x800700ce  COR_E_PATHTOOLONG
// it can be generated in two strange cases:
// - the path separator is ';' instead of ':' (':' is the correct one for the portable api)
// - the executable name is the stub "ConsoleStandalone" instead of the IL code "ConsoleStandalone.dll"

namespace raf_coreclr
{
	using namespace std::string_literals;
	using namespace raf_tools;
	//using namespace raf_helpers;

	class XCoreClrHelper
	{
		std::string _pathSeparator;
		std::string _pathSplitter;
		std::string _coreclrdll;
		std::string _publishingPath;
		coreclr_initialize_ptr _initializeFunc;
		coreclr_shutdown_ptr _shutdownFunc;
		coreclr_shutdown_2_ptr _shutdown2Func;
		coreclr_create_delegate_ptr _createDelegateFunc;
		coreclr_execute_assembly_ptr _executeAssemblyFunc;

		void* _hostHandle;
		unsigned int _domainId;
		bool _isMainAppDomainCreated;
		std::unique_ptr<raf_tools::LibraryLoader> _clrLibrary;

		std::string LogAndGetLastError()
		{
			auto message = raf_tools::GetLastError();
			raf_tools::Trace(message);
			return message;
		}

		std::string LogErrorWithResult(int res, std::string prefixmsg = ""s)
		{
			std::stringstream ss;
			if (prefixmsg.size() > 0)
			{
				ss << prefixmsg.c_str() << std::endl;
			}

			ss << std::hex << "Error 0x" << res << std::dec << std::endl;
			auto message = ss.str();
			raf_tools::Trace(message);
			auto exMessage = std::string(begin(message), end(message));
			return exMessage;
		}

		inline bool ends_with(std::string const & value, std::string const & ending)
		{
			if (ending.size() > value.size()) return false;
			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

		void InitializeFuncPointers()
		{
			_initializeFunc = _clrLibrary->GetFuncAddress<coreclr_initialize_ptr>("coreclr_initialize");
			if (_initializeFunc == nullptr)
			{
				throw std::runtime_error("Can't find coreclr_initialize function");
			}

			_shutdownFunc = _clrLibrary->GetFuncAddress<coreclr_shutdown_ptr>("coreclr_shutdown");
			if (_shutdownFunc == nullptr)
			{
				throw std::runtime_error("Can't find coreclr_shutdown function");
			}

			//_shutdown2Func = _clrLibrary->GetFuncAddress<coreclr_shutdown_2_ptr>("coreclr_shutdown_2");
			//if (_shutdown2Func == nullptr)
			//{
			//	throw std::runtime_error("Can't find coreclr_shutdown_2 function");
			//}

			_createDelegateFunc = _clrLibrary->GetFuncAddress<coreclr_create_delegate_ptr>("coreclr_create_delegate");
			if (_createDelegateFunc == nullptr)
			{
				throw std::runtime_error("Can't find coreclr_create_delegate function");
			}

			_executeAssemblyFunc = _clrLibrary->GetFuncAddress<coreclr_execute_assembly_ptr>("coreclr_execute_assembly");
			if (_executeAssemblyFunc == nullptr)
			{
				throw std::runtime_error("Can't find coreclr_execute_assembly function");
			}
		}

		void Initialize()
		{
			_isMainAppDomainCreated = false;
			auto fullCoreclrdll = _publishingPath + _coreclrdll;
			_clrLibrary = std::make_unique<raf_tools::LibraryLoader>(fullCoreclrdll, true);	// true ==> never unload the dll from memory
			InitializeFuncPointers();
		}

	public:
		std::string GetPublishingPath()
		{
			return _publishingPath;
		}

		XCoreClrHelper(const std::string& publishingPath)
		{
#if defined(WIN32) || defined(WIN64)
			_pathSeparator = "\\"s;
			_pathSplitter = ":"s;
			_coreclrdll = "coreclr.dll"s;
#else
			_pathSeparator = "/"s;
			_pathSplitter = ":"s;
			_coreclrdll = "libcoreclr.so"s;
#endif

			_publishingPath = publishingPath;
			if (!ends_with(_publishingPath, _pathSeparator))
			{
				_publishingPath += _pathSeparator;
			}

			Initialize();
		}

		~XCoreClrHelper()
		{
			UnloadAppDomain();
			//if (_pCLRRuntimeHost != nullptr)
			//{
			//	_pCLRRuntimeHost->Stop();
			//	_pCLRRuntimeHost->Release();
			//}

			//FreeLibrary(_coreClrHModule);
		}

		void CreateAppDomain(const std::string& entryPointAssembly, const std::vector<std::string>& trustedPaths)
		{
			int res;
			auto entryPointFullPathName = _publishingPath + entryPointAssembly;
			//trustedPaths.push_back(entryPointFullPathName);
			//for (const auto& assembly : trustedAssemblies)
			//{
			//	trustedPaths.push_back(_publishingPath + assembly);
			//}

			auto trusted = XUtilities::join(trustedPaths, _pathSplitter);

			// Check whether we are enabling server GC (off by default)
			const char* useServerGc = XUtilities::GetEnvValueBoolean(serverGcVar);

			// Check Globalization Invariant mode (false by default)
			const char* globalizationInvariant = XUtilities::GetEnvValueBoolean(globalizationInvariantVar);


			// https://github.com/dotnet/coreclr/blob/master/src/coreclr/hosts/coreconsole/coreconsole.cpp
			const char* property_keys[] =
			{
				"APPBASE",
				"TRUSTED_PLATFORM_ASSEMBLIES",
				"APP_PATHS",

				"APP_NI_PATHS",
				"NATIVE_DLL_SEARCH_DIRECTORIES",
				//"PLATFORM_RESOURCE_ROOTS",
				//"AppDomainCompatSwitch"

				"System.GC.Server",
				"System.Globalization.Invariant",
			};

			const char* property_values[] = {
				_publishingPath.c_str(),	// APPBASE (The base path of the application from which the exe and other assemblies will be loaded)
				"",	// trusted.c_str(),		// TRUSTED_PLATFORM_ASSEMBLIES (The list of complete paths to each of the fully trusted assemblies)
				trusted.c_str(), // _publishingPath.c_str(),			// APP_PATHS (The list of paths which will be probed by the assembly loader)

				trusted.c_str(),			// APP_NI_PATHS (The list of additional paths that the assembly loader will probe for ngen images)
				trusted.c_str(),			// NATIVE_DLL_SEARCH_DIRECTORIES (The list of paths that will be probed for native DLLs called by PInvoke)

				useServerGc,
				globalizationInvariant,
			};

			int nprops = sizeof(property_keys) / sizeof(wchar_t*);

			//DWORD appDomainFlags = APPDOMAIN_ENABLE_PLATFORM_SPECIFIC_APPS | APPDOMAIN_ENABLE_PINVOKE_AND_CLASSIC_COMINTEROP;

			_hostHandle = nullptr;
			_domainId = 0;
			res = _initializeFunc(
				entryPointFullPathName.c_str(),
				"CoreClrAppDomain",
				nprops,
				property_keys,
				property_values,
				&_hostHandle,
				&_domainId);

			if (!SUCCEEDED(res))
			{
				throw std::runtime_error(LogErrorWithResult(res, "coreclr_initialize failed"));
			}

			_isMainAppDomainCreated = true;
		}

		bool UnloadAppDomain()
		{
			if (_isMainAppDomainCreated && _domainId != 0 && _clrLibrary != nullptr)
			{
				int res = _shutdownFunc(_hostHandle, _domainId);
				if (res)
				{
					throw std::runtime_error(LogErrorWithResult(res, "coreclr_shutdown failed"));
				}

				_clrLibrary = nullptr;
				return true;
			}

			return false;
		}

		void* CreateDelegate(const std::string& assembly, const std::string& type, const std::string& method)
		{
			void* del;
			auto res = _createDelegateFunc(
				_hostHandle,
				_domainId,
				assembly.c_str(),
				type.c_str(),
				method.c_str(),
				&del);

			if (res)
			{
				throw std::runtime_error(LogErrorWithResult(res, "coreclr_create_delegate failed"));
			}

			return del;
		}


	};

	// the .NET Framework typecode enumeration
	enum typeCode
	{
		//
		// Summary:
		//     A null reference.
		TypeCodeEmpty = 0,

		//
		// Summary:
		//     A general type representing any reference or value type not explicitly represented
		//     by another TypeCode.
		TypeCodeObject = 1,

		//
		// Summary:
		//     A simple type representing Boolean values of true or false.
		TypeCodeBoolean = 3,

		//
		// Summary:
		//     An integral type representing unsigned 16-bit integers with values between 0
		//     and 65535. The set of possible values for the System.TypeCode.Char type corresponds
		//     to the Unicode character set.
		TypeCodeChar = 4,

		//
		// Summary:
		//     An integral type representing signed 8-bit integers with values between -128
		//     and 127.
		TypeCodeSByte = 5,

		//
		// Summary:
		//     An integral type representing unsigned 8-bit integers with values between 0 and
		//     255.
		TypeCodeByte = 6,

		//
		// Summary:
		//     An integral type representing signed 16-bit integers with values between -32768
		//     and 32767.
		TypeCodeInt16 = 7,

		//
		// Summary:
		//     An integral type representing unsigned 16-bit integers with values between 0
		//     and 65535.
		TypeCodeUInt16 = 8,

		//
		// Summary:
		//     An integral type representing signed 32-bit integers with values between -2147483648
		//     and 2147483647.
		TypeCodeInt32 = 9,

		//
		// Summary:
		//     An integral type representing unsigned 32-bit integers with values between 0
		//     and 4294967295.
		TypeCodeUInt32 = 10,

		//
		// Summary:
		//     An integral type representing signed 64-bit integers with values between -9223372036854775808
		//     and 9223372036854775807.
		TypeCodeInt64 = 11,

		//
		// Summary:
		//     An integral type representing unsigned 64-bit integers with values between 0
		//     and 18446744073709551615.
		TypeCodeUInt64 = 12,

		//
		// Summary:
		//     A floating point type representing values ranging from approximately 1.5 x 10
		//     -45 to 3.4 x 10 38 with a precision of 7 digits.
		TypeCodeSingle = 13,

		//
		// Summary:
		//     A floating point type representing values ranging from approximately 5.0 x 10
		//     -324 to 1.7 x 10 308 with a precision of 15-16 digits.
		TypeCodeDouble = 14,

		//
		// Summary:
		//     A simple type representing values ranging from 1.0 x 10 -28 to approximately
		//     7.9 x 10 28 with 28-29 significant digits.
		TypeCodeDecimal = 15,

		//
		// Summary:
		//     A type representing a date and time value.
		TypeCodeDateTime = 16,

		//
		// Summary:
		//     A sealed class type representing Unicode character strings.
		TypeCodeString = 18
	};

}