#pragma once
// Windows-specific include file

#include <string>
#include <sstream>
#include <memory>
#include <Windows.h>

namespace raf_helpers
{
    struct Utilities
    {
        static inline void Trace(const std::wstring& msg, bool crlf = true)
        {
#if defined(_DEBUG) || defined(_DEBUGRAF)
            OutputDebugString(msg.c_str());
            if (crlf)
                OutputDebugString(L"\r\n");
#else
            UNREFERENCED_PARAMETER(msg);
            UNREFERENCED_PARAMETER(crlf);
#endif
        }

        static inline void Trace(const std::string& msg, bool crlf = true)
        {
#if defined(_DEBUG) || defined(_DEBUGRAF)
            OutputDebugStringA(msg.c_str());
            if (crlf)
                OutputDebugStringA("\r\n");
#else
            UNREFERENCED_PARAMETER(msg);
            UNREFERENCED_PARAMETER(crlf);
#endif
        }

        static std::string Format(bool appendLastError, bool useCrLf, LPCSTR format, ...)
        {
            DWORD lastError = 0;
            std::string lastErrorMessage;
            if (appendLastError)
            {
                lastError = GetLastError();
                lastErrorMessage = GetErrorMessageA(lastError);
                lastErrorMessage = Format(false, false, "[0x%08x] %s", lastError, lastErrorMessage.c_str());
            }

            va_list args;
            va_start(args, format);
            auto length = _vscprintf(format, args) + 1;
            auto buffer = std::make_unique<char[]>(length);

            _vsnprintf_s(buffer.get(), length, _TRUNCATE, format, args);
            std::string res(buffer.get());
            if (appendLastError)
            {
                res += useCrLf ? "\r\n" : " " + lastErrorMessage;
            }
            return res;
        }

        static std::wstring Format(bool appendLastError, bool useCrLf, LPCWSTR format, ...)
        {
            DWORD lastError = 0;
            std::wstring lastErrorMessage;
            if (appendLastError)
            {
                lastError = GetLastError();
                lastErrorMessage = GetErrorMessageW(lastError);
                lastErrorMessage = Format(false, false, L"[0x%08x] %s", lastError, lastErrorMessage.c_str());
            }

            va_list args;
            va_start(args, format);
            auto length = _vscwprintf(format, args) + 1;
            auto buffer = std::make_unique<wchar_t[]>(length);

            _vsnwprintf_s(buffer.get(), length, _TRUNCATE, format, args);
            std::wstring res(buffer.get());
            if (appendLastError)
            {
                res += useCrLf ? L"\r\n" : L" " + lastErrorMessage;
            }
            return res;
        }

        static std::string GetErrorMessageA(DWORD lastError)
        {
            std::string message;
            LPSTR pMsg;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsg, 0, NULL);
            message.append(pMsg);
            LocalFree((LPVOID)pMsg);

            message.erase(message.find_last_not_of(" \n\r\t") + 1);    // trim right the special chars and whitespaces
            return message;
        }

        static std::wstring GetErrorMessageW(DWORD lastError)
        {
            std::wstring message;
            LPWSTR pMsg;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsg, 0, NULL);
            message.append(pMsg);
            LocalFree((LPVOID)pMsg);

            message.erase(message.find_last_not_of(L" \n\r\t") + 1);    // trim right the special chars and whitespaces
            return message;
        }

        static std::wstring GetMessageForHResult(HRESULT hr, std::wstring prefixmsg = L"")
        {
            std::wstring message = prefixmsg;
            WORD facility = HRESULT_FACILITY(hr);
            //CComPtr<IErrorInfo> spErrorInfo;
            //HRESULT hrLocal = GetErrorInfo(0, &spErrorInfo);
            //if (hrLocal == S_OK && spErrorInfo != NULL)
            //{
            //	BSTR description;
            //	hrLocal = spErrorInfo->GetDescription(&description);
            //	message.append((LPWSTR)description);
            //	SysFreeString(description);
            //}
            //else
            if (facility == FACILITY_ITF)
            {
                message.append(L"Custom error: ");
                std::wstringstream ss;
                ss << L"0x" << std::hex << hr << std::dec;
                message.append(ss.str());
            }
            else
            {
                LPWSTR pMsg;
                FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, (DWORD)hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsg, 0, NULL);
                if (pMsg != nullptr)
                {
                    message.append(pMsg);
                    LocalFree((LPVOID)pMsg);
                }
                else
                {
                    std::wstringstream ss;
                    ss << L"0x" << std::hex << hr << std::dec;
                    message.append(ss.str());
                }
            }
            return message;
        }

        static void _MakeError57()
        {
            CreateFile(nullptr, 0, 0, nullptr, 0, 0, nullptr);
        }

        static void SetLastError(uint32_t err)
        {
            ::SetLastError(err);
        }
    };
}