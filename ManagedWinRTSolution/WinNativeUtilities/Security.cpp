#include "pch.h"
#include "Security.h"
#include "Security.g.cpp"

#include <Windows.h>
#include <sddl.h>

namespace winrt::WinNativeUtilities::implementation
{
    int32_t Security::CreateLowProcess(hstring const& fileName, hstring const& commandLine)
    {
        // Low integrity SID
        WCHAR IntegritySid[20] = L"S-1-16-4096";
        DWORD LastError = 0;

        HANDLE hToken = nullptr;
        // Read the current process token
        if (OpenProcessToken(GetCurrentProcess(),
            TOKEN_DUPLICATE | TOKEN_ADJUST_DEFAULT | TOKEN_QUERY | TOKEN_ASSIGN_PRIMARY,
            &hToken))
        {
            HANDLE hNewToken = nullptr;
            // Duplicate the current token so that we can modify it
            if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr,
                SecurityImpersonation, TokenPrimary, &hNewToken))
            {
                PSID pIntegritySid = nullptr;
                // converts the sid string into a binary structure
                if (ConvertStringSidToSid(IntegritySid, &pIntegritySid))
                {
                    TOKEN_MANDATORY_LABEL TIL = { 0 };
                    TIL.Label.Attributes = SE_GROUP_INTEGRITY;
                    TIL.Label.Sid = pIntegritySid;

                    // Modify the integrity level of the new token
                    if (SetTokenInformation(hNewToken, TokenIntegrityLevel, &TIL,
                        sizeof(TOKEN_MANDATORY_LABEL)))
                    {
                        PROCESS_INFORMATION ProcInformation = { 0 };
                        STARTUPINFO StartupInfo = { 0 };
                        StartupInfo.cb = sizeof(StartupInfo);

                        // creates the process using the new token
                        if (CreateProcessAsUser(hNewToken,
                            fileName.c_str(),
                            const_cast<wchar_t*>(commandLine.c_str()),
                            nullptr,
                            nullptr,
                            FALSE,
                            0,
                            nullptr,
                            nullptr,
                            &StartupInfo,
                            &ProcInformation))
                        {
                            if (ProcInformation.hProcess != nullptr)
                                CloseHandle(ProcInformation.hProcess);

                            if (ProcInformation.hThread != nullptr)
                                CloseHandle(ProcInformation.hThread);
                        }
                        else
                            LastError = ::GetLastError();
                    }
                    else
                        LastError = ::GetLastError();
                    LocalFree(pIntegritySid);
                }
                else
                    LastError = ::GetLastError();
                CloseHandle(hNewToken);
            }
            else
                LastError = ::GetLastError();
            CloseHandle(hToken);
        }
        else
            LastError = ::GetLastError();

        return LastError;
    }













    int32_t Security::MyProperty()
    {
        return _myProperty;
    }

    void Security::MyProperty(int32_t value)
    {
        _myProperty = value;
    }
}
