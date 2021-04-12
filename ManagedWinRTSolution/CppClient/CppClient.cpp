#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include <Windows.h>

using namespace std;

// This project exists to the sole purpose of finding the
// lower bound of execution timings for the benchmark

struct WindowInfo
{
    int32_t HWnd;
    wstring Title;
    bool IsVisible;
};


struct Windowing
{
    Windowing()
    {
    }

    std::vector<WindowInfo> EnumWindows()
    {
        std::vector<WindowInfo> vec;
        ::EnumWindows(enumWindowCallback, (LPARAM)this);
        return vec;
    }

    void EnumWindowsNull()
    {
        vec.clear();
        ::EnumWindows(enumWindowCallbackNull, (LPARAM)this);
    }


private:

    std::vector<WindowInfo> vec;
    uint32_t bufferSize = 0;
    std::unique_ptr<wchar_t[]> buffer;

    static BOOL CALLBACK enumWindowCallbackNull(HWND hWnd, LPARAM lparam)
    {
        Windowing* p = (Windowing*)lparam;
        return true;
    }

    static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
        Windowing* p = (Windowing*)lparam;
        UINT32 bufferLength = GetWindowTextLength(hWnd);    // effective text

        if (p->bufferSize < bufferLength)
        {
            p->bufferSize = bufferLength + 1;   // null term.
            p->buffer.reset(new wchar_t[p->bufferSize]);
        }

        int res = GetWindowText(hWnd, p->buffer.get(), p->bufferSize);

        if (res == 0 && bufferLength != 0)
        {
            //auto extended = GetLastError();
            //auto msg = raf_helpers::Utilities::Format(true, false, "GetWindowText failed");
            ////return HRESULT_FROM_WIN32(GetLastError())
            ////throw runtime_error(msg);
            
            //cout << "error" << endl;
            return true;
        }

        WindowInfo wi;
        wi.HWnd = (int32_t)(int64_t)hWnd;
        wi.Title = wstring(p->buffer.get(), bufferLength);
        wi.IsVisible = IsWindowVisible(hWnd);
        p->vec.push_back(wi);

        return true;
    }
};


int main()
{
    Windowing w;
    auto start = chrono::steady_clock::now();

    for (int i = 0; i < 400; i++)
    {
        w.EnumWindows();
        //w.EnumWindowsNull();
    }

    auto end = chrono::steady_clock::now();

    cout << "Elapsed: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
}

