#pragma once

#include <memory>
#include "Windowing.g.h"
#include "RafUtilities.h"

namespace winrt::WinNativeUtilities::implementation
{
    struct Windowing : WindowingT<Windowing>
    {
        Windowing()
        {
        }

        //com_array<hstring> EnumWindows()
        Windows::Foundation::Collections::IVectorView<WindowInfo> EnumWindows()
        {
            std::vector<WindowInfo> vec;
            ::EnumWindows(enumWindowCallback, (LPARAM)&vec);
            //com_array<hstring> result(vec);

            Windows::Foundation::Collections::IVectorView<WindowInfo> result
            {
                winrt::single_threaded_vector(std::move(vec)).GetView() 
            };

            return result;
        }

    private:

        static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
            std::vector<WindowInfo>* pvec = (std::vector<WindowInfo>*)lparam;
            UINT32 bufferLength = GetWindowTextLength(hWnd);    // effective text
            UINT32 totalBufferLength = bufferLength + 1;        // null terminated

            std::unique_ptr<wchar_t[]> buffer(new wchar_t[totalBufferLength]);

            int res = GetWindowText(hWnd, buffer.get(), totalBufferLength);
            
            if (res == 0 && bufferLength != 0)
            {
                //auto extended = GetLastError();
                //auto msg = raf_helpers::Utilities::Format(true, false, "GetWindowText failed");
                //return HRESULT_FROM_WIN32(GetLastError())
                //throw runtime_error(msg);
                return true;
            }

            WindowInfo wi;
            wi.HWnd = (int32_t)(int64_t)hWnd;
            wi.Title = hstring(buffer.get(), bufferLength);
            wi.IsVisible = IsWindowVisible(hWnd);
            pvec->push_back(wi);

            return true;
        }
    };
}



namespace winrt::WinNativeUtilities::factory_implementation
{
    struct Windowing : WindowingT<Windowing, implementation::Windowing>
    {
    };
}
