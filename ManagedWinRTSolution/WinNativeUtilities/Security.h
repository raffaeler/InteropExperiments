#pragma once

#include "Security.g.h"

namespace winrt::WinNativeUtilities::implementation
{
    struct Security : SecurityT<Security>
    {
        Security()
        {
            _myProperty = 0;
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        int32_t CreateLowProcess(hstring const& fileName, hstring const& commandLine);



    private:
        int32_t _myProperty;
    };
}

namespace winrt::WinNativeUtilities::factory_implementation
{
    struct Security : SecurityT<Security, implementation::Security>
    {
    };
}
